package quorum.ship.turret
{
	
	import flash.display.MovieClip;
	
	public class Blitz extends Turret
	{
		
		public function Blitz():void
		{
			super();
			roundType = "Blitz";
			rounds = 50;
			coolDown = 200;
			jerkVal = -0.03;
			rangeOfMotion = 30;
			speedOfMotion = 10;
			tip_mc = tipmc;
			chamber_mc = chambermc;
			setup();
		}
		
	}
	
}