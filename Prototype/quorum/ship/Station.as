package quorum.ship {
	
	import flash.display.MovieClip;
	
	import quorum.ship.turret.*;
	
	public class Station extends MovieClip {
		
		private var available:Boolean = true;
		
		private var leftStation:Station = null;
		private var rightStation:Station = null;
		
		public static const EMPTY:int = 0;
		public static const SWIVEL:int = 1;
		public static const BLITZ:int = 2;
		public static const BLAZE:int = 3;
		public static const HEAVY:int = 4;
		public static const SHIELD:int = 5;
		public static const ENGINEER:int = 6;
		public static const PILOT = 7;
		private var stationType:int = EMPTY;
		
		public var turret:Turret = null;
		
		public function Station(st:int = 0):void {
			stationType = st;
			switch (stationType)
			{
				case SWIVEL:
					turret = new Swivel();
					addChild(turret);
					turret.x = 42;
					break;
				case BLITZ:
					turret = new Blitz();
					addChild(turret);
					turret.x = 42;
					break;
				case BLAZE:
					turret = new Blaze();
					addChild(turret);
					turret.x = 42;
					break;
				case HEAVY:
					turret = new Heavy();
					addChild(turret);
					turret.x = 42;
					break;					
			}
		}
		
		public function setNextStation(direction:Number, station:Station):void {
			if(direction > 0){
				rightStation = station;
			} else {
				leftStation = station;
			}
		}
		
		public function getNextStation(direction:Number):Station {
			if(direction > 0){
				return rightStation;
			} else {
				return leftStation;
			}
		}
		
		public function get isAvailable():Boolean { return available; }
		public function set isAvailable(val:Boolean):void { available = val; }
		
		public function get StationType():int { return stationType; };
		
	}
	
}