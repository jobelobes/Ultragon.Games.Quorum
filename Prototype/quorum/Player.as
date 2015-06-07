package quorum {
	
	import flash.display.MovieClip;
	
	import flash.geom.Point;
	
	import quorum.ship.Station;
	
	import quorum.xbox.XBox360Controller;
	import quorum.xbox.events.*;
	
	public class Player extends MovieClip {
		
		private var controller:XBox360Controller;
		
		private var qd:QuorumDocument;
		private var lastPos:Point = new Point();
		
		private const DEGRAD:Number = Math.PI/180.0;
		
		public var currentStation:Station = null;
		private var targetStation:Station = null;
		
		private var playerState:int;
		private static const SEATED:int = 0;
		private static const WALKING:int = 1;
		private static const RUNNING:int = 2;
		
		private var walkSpeed = 5;
		private var runSpeed = 5;
		
		//private var station:Station;
		
		public function Player(cont:XBox360Controller):void {
			qd = QuorumDocument.getInstance();
			controller = cont;
			playerState = WALKING;
			
			controller.addEventListener(XBox360AnalogEvent.ANALOG_MOVE, onAnalog);
			controller.addEventListener(XBox360ButtonEvent.BUTTON_PRESS, onButton);
		}
		
		private function onButton(e:XBox360ButtonEvent):void {
			switch(playerState)
			{
				case WALKING:
					switch(e.buttonIndex)
					{
						case XBox360Controller.A:
							seatPlayer();
							break;
					}
					break;
				case SEATED:
					switch(e.buttonIndex)
					{
						case XBox360Controller.Y:
							standUp();
							break;
						case XBox360Controller.LEFT_BUMPER:
							swapSeat(-1);
							break;
						case XBox360Controller.RIGHT_BUMPER:
							swapSeat(1);
							break;
					}
					break;
			}
		}
		
		private function seatPlayer():void {
			targetStation = null;
			currentStation = qd.seatPlayer(this);
			if(currentStation != null){
				x = currentStation.x;
				y = currentStation.y;
				playerState = SEATED;
			} else {
				playerState = WALKING;
			}
		}
		
		public function standUp():void {
			currentStation.isAvailable = true;
			currentStation = null;
			playerState = WALKING;			
		}
		
		private function swapSeat(direction:Number):void {
			targetStation = currentStation.getNextStation(direction);
			standUp();
			playerState = RUNNING;
		}
		
		private function onAnalog(e:XBox360AnalogEvent):void {
			switch(e.analogIndex)
			{
				case XBox360Controller.LEFT_TRIGGER:
					//qd.setShipZoom(controller.analogPosition(XBox360Controller.LEFT_TRIGGER));
					break;
				case XBox360Controller.RIGHT_TRIGGER:
					//qd.setWorldZoom(controller.analogPosition(XBox360Controller.RIGHT_TRIGGER));
					break;
			}
		}
		
		public function stepFrame():void {
			lastPos.x = x;
			lastPos.y = y;
			switch(playerState){
				case WALKING:
					var translation:Point = controller.stickAsPoint(XBox360Controller.LEFT);
					var relativeAngle:Number = (Math.atan2(translation.y, translation.x))-(parent.rotation*DEGRAD);
					translation = Point.polar(translation.length, relativeAngle);
					translation.normalize(walkSpeed * translation.length);
					x += translation.x;
					y += translation.y;
					break;
				case SEATED:
					x = currentStation.x;
					y = currentStation.y;
					performStation();
					break;
				case RUNNING:
					var vectorToTargetSeat:Point = Point.polar(runSpeed,Math.atan2(targetStation.y-y,targetStation.x-x));
					x += vectorToTargetSeat.x;
					y += vectorToTargetSeat.y;
					if(targetStation.hitTestPoint(parent.x+this.x, parent.y+this.y, true))
					{
						seatPlayer();
					}
					break;
			}
		}
		
		public function performStation():void {
			switch(currentStation.StationType)
			{
				case Station.PILOT:
					qd.fly(controller);
					break;
				default:
					qd.shoot(controller, currentStation);
					break;
			}
		}
		
		public function stayOnDeck():void {
			lastPos.x = x;
			lastPos.y = y;
			lastPos.normalize(qd.ship.shipFloor_mc.width/2);
			x = lastPos.x;
			y = lastPos.y;
		}
		
	}
	
}