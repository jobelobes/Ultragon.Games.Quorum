package quorum.ship.turret
{
	
	import quorum.QuorumDocument;
	
	import flash.display.MovieClip;
	
	import flash.events.TimerEvent;
	
	import flash.geom.Point;
	
	import flash.utils.Timer;
	
	public class Turret extends MovieClip
	{
		
		private const DEGRAD:Number = Math.PI/180;
		
		protected var roundType:String;
		protected var rounds:int;
		protected var coolDown:Number;
		
		protected var rangeOfMotion:Number;
		protected var speedOfMotion:Number;
		
		protected var pool:Array;
		protected var live:Array;
		protected var cool:Array;
		
		private var coolTimer:Timer;
		
		protected var heading:Number;
		
		protected var tip_mc:MovieClip;
		protected var tip:Point;
		
		protected var chamber_mc:MovieClip;
		protected var chamber:Point;
		
		private var firingAllowed:Boolean = true;
		
		protected var jerkVal:Number = 0;
		
		public function Turret():void {
			
		}
		
		public function setup():void 
		{
			generatePool();
			setupTimer();
		}
		
		private function generatePool():void {
			if(pool || cool || live)
			{
				killArrays();
			}
			cool = new Array();
			pool = new Array();
			live = new Array();
			
			for (var i:int = 0; i < rounds; i++)
			{
				switch(roundType)
				{
					case "Swivel":
						pool.push(new SwivelBullet(this));
						break;
					case "Blitz":
						pool.push(new BlitzBullet(this));
						break;
					case "Blaze":
						pool.push(new BlazeBullet(this));
						break;
					case "Heavy":
						pool.push(new HeavyBullet(this));
						break;
				}
			}
		}
		
		private function setupTimer():void {
			coolTimer = new Timer(coolDown);
			coolTimer.addEventListener(TimerEvent.TIMER, recycle);
			coolTimer.start();
		}
		
		public function nextRound():Bullet {
			if(pool.length > 0 && firingAllowed)
			{
				var b = Bullet(pool.pop());
				live.push(b);
				return b;
			}
			else {
				return null;
			}
		}
		
		public function pan(dir:Point, fire:Boolean=false):void {
			if(dir.x != 0)
			{
				var blazeMult:Number = 1;
				if(fire == true && roundType == "Blaze"){
					blazeMult = .5;
				}
				var lr:Number = dir.x/Math.abs(dir.x);
				rotation += dir.length * lr * speedOfMotion * blazeMult;
				rotation = Math.abs(rotation) > rangeOfMotion ? rangeOfMotion * rotation/Math.abs(rotation) : rotation;
			}
		}
		
		public function fire():void {
			var b:Bullet = nextRound();
			
			if(b)
			{
				tip = this.localToGlobal(new Point(tip_mc.x, tip_mc.y));
				chamber = this.localToGlobal(new Point(chamber_mc.x, chamber_mc.y));
				heading = Math.atan2(tip.y - chamber.y, tip.x - chamber.x) / DEGRAD;
				b.x = tip.x;
				b.y = tip.y;
				b.rotation = heading;
				stage.addChild(b);
				b.fire();
				QuorumDocument.getInstance().ship.jerk(Point.polar(jerkVal, (heading + 180)*DEGRAD),parent.localToGlobal(new Point(this.x, this.y)));
			}
		}
		
		public function queue(aBullet:Bullet):void {
			var io:int = live.indexOf(aBullet);
			if(io > -1) {
				cool.push(live.splice(io, 1)[0]);
			}
			if(roundType == "Blaze" && pool.length == 0){
				firingAllowed = false;
			}
		}
		
		private function recycle(e:TimerEvent):void {
			if (cool.length > 0) {
				pool.push(cool.pop());
			}
			if(roundType == "Blaze" && cool.length == 0)
			{
				firingAllowed = true;
			}
		}
		
		private function killArrays():void
		{
			
		}
		
	}
}