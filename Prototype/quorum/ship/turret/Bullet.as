package quorum.ship.turret
{
	import flash.display.MovieClip;
	
	import flash.events.Event;
	
	import flash.geom.Point;
	
	public class Bullet extends MovieClip
	{
		
		private const DEGRAD:Number = Math.PI/180;
		
		protected var range:Number;
		private var distance:Number;
		protected var damage:Number;
		protected var muzzleVelocity:Number;
		private var translation:Point;
		
		private var isActive:Boolean = false;
		
		private var owner:Turret;
		
		public function Bullet(_owner:Turret):void {
			owner = _owner;
		}
		
		public function fire():void {
			distance = 0;
			translation = new Point(Math.cos(rotation*DEGRAD)*muzzleVelocity, Math.sin(rotation*DEGRAD)*muzzleVelocity);
			isActive = true;
			addEventListener(Event.ENTER_FRAME, update);
		}
		
		private function update(e:Event):void {
			x += translation.x;
			y += translation.y;
			distance += muzzleVelocity;
			
			if(distance >= range)
			{
				dissipate();
			}
		}
		
		private function dissipate():void {
			isActive = false;
			if(parent)
			{
				parent.removeChild(this);
			}
			owner.queue(this);
			removeEventListener(Event.ENTER_FRAME, update);
		}
		
	}
}