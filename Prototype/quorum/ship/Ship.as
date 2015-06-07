package quorum.ship
{

	import flash.display.MovieClip;

	import flash.geom.Point;

	public class Ship extends MovieClip
	{

		public var stations:Array = new Array();
		
		private const DEGRAD:Number = Math.PI/180.0;
		
		private var heading:Number = 0;
		private var movementVector:Point = new Point();
		
		private var rotSpeed:Number = 0;
		private var maxRot:Number = 10;
		
		private var maxSpeed:Number = 15;
		private var acc:Number = 2;
		
		private var impulseArray:Array = new Array();
		private var impRots:Array = new Array();
		
		private var killingRot:Boolean = false;
		private var frameCount:Number = -1;
		
		public function get MovementVector():Point { return movementVector; };

		public function Ship():void
		{
			var heading:Number;
			var coord:Point;
			var tempStation:Station = new Station(Station.PILOT);
			
			tempStation.rotation = 0;
			addChild(tempStation);
			stations.push(tempStation);
			
			var lastStation:Station=tempStation;
			for (var i:int = 0; i<6; i++)
			{
				heading = 30 + 60*i;
				if(i%4==0){
					tempStation = new Station(Station.SWIVEL);
				} else if(i%4==1){
					tempStation = new Station(Station.BLITZ);
				} else if(i%4==2){
					tempStation = new Station(Station.BLAZE);
				}else {
					tempStation = new Station(Station.HEAVY);
				}
				addChild(tempStation);
				coord = Point.polar(60, heading*DEGRAD);
				tempStation.x += coord.x;
				tempStation.y += coord.y;
				tempStation.rotation = heading;
				stations.push(tempStation);
				tempStation.setNextStation(-1,lastStation);
				lastStation.setNextStation(1,tempStation);
				lastStation = tempStation;
			}
			
			lastStation.setNextStation(1,stations[1]);
			stations[1].setNextStation(-1,lastStation);
			stations[0].setNextStation(-1,stations[4]);
			stations[0].setNextStation(1,stations[6]);
			
			
			var imp:Burner;
			var trans:Point;
			var rot:Number;
			var impRot:Point;
			for (var j:int = 0; j < 36; j++)
			{
				imp = new Burner();
				rot = (360/36)*j;
				trans = Point.polar(105, rot*DEGRAD);
				imp.x = trans.x;
				imp.y = trans.y;
				imp.rotation = rot+180;
				imp.scaleX = imp.scaleY = 0.5;
				addChildAt(imp,0);
				impulseArray.push(imp);
				trans.normalize(1);
				impRots.push(trans);
			}
		}
		
		public function shipPhysics():void {
			heading += rotSpeed;
			rotation = heading;
		}
		
		public function thrust(accLeft:Number, accRight:Number):void {
			if(!killingRot)
			{
				lefta_mc.gotoAndStop(Math.ceil(accLeft*100));
				leftb_mc.gotoAndStop(Math.ceil(accLeft*100));
				
				righta_mc.gotoAndStop(Math.ceil(accRight*100));
				rightb_mc.gotoAndStop(Math.ceil(accRight*100));
				
				var disparity:Number = accLeft - accRight;
				var straight:Number = accLeft < accRight ? accLeft : accRight;
				
				//var dir:Point = Point.polar(straight*acc, heading*DEGRAD);
				rotSpeed += Math.atan(disparity/350)/DEGRAD;
				rotSpeed = Math.abs(rotSpeed) > maxRot ? maxRot * Math.abs(rotSpeed)/rotSpeed : rotSpeed;
				
				//dir = dir.add(Point.polar(disparity, heading*DEGRAD));
				
				//movementVector.offset(-dir.x*acc, -dir.y*acc);
				//movementVector.length>maxSpeed?movementVector.normalize(maxSpeed):null;
			}
			else
			{
				rotSpeed *= 0.8;
				frameCount--;
				if(frameCount <= 0)
				{
					killingRot = false;
					rotSpeed = 0;
				}
				var overall = 1-Math.abs(rotSpeed)/maxRot;
				if(rotSpeed > 0)
				{
					righta_mc.gotoAndStop(Math.ceil(overall*100));
					rightb_mc.gotoAndStop(Math.ceil(overall*100));
				}
				else if (rotSpeed < 0)
				{
					lefta_mc.gotoAndStop(Math.ceil(overall*100));
					leftb_mc.gotoAndStop(Math.ceil(overall*100));
				}
				else
				{
					lefta_mc.gotoAndStop(1);
					leftb_mc.gotoAndStop(1);
					
					righta_mc.gotoAndStop(1);
					rightb_mc.gotoAndStop(1);
				}
			}
		}
		
		public function jerk(fire:Point, myPos:Point):void {
			var perp:Point = new Point(myPos.x - x, myPos.y - y);
			perp.normalize(.2);
			
			var tang:Point = new Point(perp.y, -perp.x);
			
			var perpVel:Number = dot(fire,perp);
			var tangVel:Number = dot(fire,tang);
			
			perp.normalize(perpVel);
			
			movementVector.offset(perp.x, perp.y);
			movementVector.length>maxSpeed?movementVector.normalize(maxSpeed):null;
			
			rotSpeed += tangVel;
			rotSpeed = Math.abs(rotSpeed) > maxRot ? maxRot * Math.abs(rotSpeed)/rotSpeed : rotSpeed;
		}
		
		public function killRot():void {
			if(!killingRot)
			{
				killingRot = true;
				frameCount = 10;
			}
		}
		
		public function impulse(dir:Point):void {
			dir.x = -dir.x;
			dir.y = -dir.y;
			var relativeAngle:Number = (Math.atan2(dir.y, dir.x))-(heading*DEGRAD);
			var relativeDir:Point = Point.polar(dir.length, relativeAngle);
			movementVector.offset(dir.x*acc, dir.y*acc);
			movementVector.length>maxSpeed?movementVector.normalize(maxSpeed):null;
			
			var val:Number;
			for (var i:int = 0; i < impulseArray.length; i++)
			{
				val = dot(relativeDir, impRots[i]);
				impulseArray[i].gotoAndStop(Math.ceil(val*val*val*100));
			}
		}
		
		public function dot(v1:Point, v2:Point)
		{
			return v1.x*v2.x + v1.y*v2.y;
		}

	}

}