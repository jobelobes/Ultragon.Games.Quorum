package quorum.ship.turret
{
	
	import flash.display.MovieClip;
	
	public class Blaze extends Turret
	{
		
		public function Blaze():void
		{
			super();
			roundType = "Blaze";
			rounds = 50;
			coolDown = 100;
			rangeOfMotion = 30;
			speedOfMotion = 5;
			tip_mc = tipmc;
			chamber_mc = chambermc;
			setup();
		}
		
	}
	
}