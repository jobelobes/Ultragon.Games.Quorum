package quorum
{

	import quorum.xbox.*;
	import quorum.ship.Ship;
	import quorum.ship.Station;
	
	import flash.display.MovieClip;

	import flash.events.Event;
	
	import flash.geom.Point;

	public class QuorumDocument extends MovieClip
	{
		private static var _instance:QuorumDocument;

		private var manager:XBox360Manager;
		
		public var ship:Ship;
		private var players:Array = new Array();

		private var stars:Array = new Array();
		
		private var sh:Number;
		private var sw:Number;
		
		public function QuorumDocument():void
		{
			_instance = this;
			manager = XBox360Manager.getInstance();
			manager.connect();
			
			sh = stage.stageHeight;
			sw = stage.stageWidth;
			
			buildStarfield();
			
			ship = new Ship();
			addChild(ship);
			ship.x = stage.stageWidth/2;
			ship.y = stage.stageHeight/2;

			ship.scaleX = ship.scaleY = .5;
			
			var tempPlayer:Player;
			for (var i:int = 0; i < 4; i++)
			{
				tempPlayer = new Player(manager.getController(i));
				tempPlayer.gotoAndStop(i+1);
				ship.addChild(tempPlayer);
				players.push(tempPlayer);
			}

			addEventListener(Event.ENTER_FRAME, gameLoop);
		}
		
		public function buildStarfield():void {
			var star:Star;
			for (var s:int = 0; s < 100; s++)
			{
				star = new Star();
				star.x = Math.random()*sw;
				star.y = Math.random()*sh;
				star.scaleX = star.scaleY = Math.random()+0.5;
				addChild(star);
				stars.push(star);
			}
		}
		
		public static function getInstance():QuorumDocument
		{
			return _instance;
		}

		private function gameLoop(e:Event):void
		{
			var player:Player;
			var globalPos:Point;
			for (var i:int = 0; i<players.length; i++)
			{
				player = players[i];
				globalPos = player.parent.localToGlobal(new Point(player.x, player.y));
				players[i].stepFrame();
				if(!ship.shipFloor_mc.hitTestPoint(globalPos.x, globalPos.y, true)){
					player.stayOnDeck();
				}
			}
			
			ship.shipPhysics();
			moveStars(ship.MovementVector);
		}
		
		private function moveStars(vec:Point):void
		{
			for each (var star:MovieClip in stars)
			{
				star.x += vec.x;
				star.y += vec.y;
				
				star.x = star.x < 0 ? sw + star.x : star.x > sw ? star.x - sw : star.x;
				star.y = star.y < 0 ? sh + star.y : star.y > sh ? star.y - sh : star.y;
			}
		}
		
		//stuff called by other classes
		public function setShipZoom(to:Number):void {
			ship.scaleX = ship.scaleY = 1 + to; // is three? lol
		}
		
		public function setWorldZoom(to:Number):void {
			y = 300 * to;
			x = 200 * to;
			scaleX = scaleY = 0.25 + (1 - to)*0.75; // is three? lol
		}
		
		public function seatPlayer(player:Player):Station {
			var station:Station;
			var globalPos:Point = player.localToGlobal(new Point(player.x, player.y));
			for(var i:int = 0; i<ship.stations.length; i++){
				station = ship.stations[i];
				if(station.isAvailable){
					if(station.hitTestObject(player)){
						station.isAvailable = false;
						return station;
					}
				} else {
					if(station.hitTestObject(player)){
						for(var j:int = 0; j<players.length; j++){
							if(players[j].currentStation == station)
							{
								players[j].standUp();
								station.isAvailable = false;
								players[j].x = player.x;
								players[j].y = player.y;
								return station;
							}
						}
					}
				}
			}
			return null;
		}
		
		public function fly(activeController:XBox360Controller):void
		{
			ship.thrust(activeController.analogPosition(XBox360Controller.LEFT_TRIGGER),
						activeController.analogPosition(XBox360Controller.RIGHT_TRIGGER));
			
			if(activeController.isDown(XBox360Controller.A))
			{
				ship.impulse(activeController.stickAsPoint(XBox360Controller.LEFT_STICK));
			}
			else
			{
				ship.impulse(new Point());
			}
			
			if(activeController.isDown(XBox360Controller.X))
			{
				ship.killRot();
			}
		}
		
		public function shoot(activeController:XBox360Controller, aStation:Station):void
		{
			if(aStation.turret)
			{
				if(activeController.isDown(XBox360Controller.A))
				{
					aStation.turret.fire();
				}
				
				aStation.turret.pan(activeController.stickAsPoint(XBox360Controller.LEFT_STICK));
			}
		}
		
		//accessors
	}
}