package quorum.ship.turret
{
	
	import flash.display.MovieClip;
	
	public class Heavy extends Turret
	{
		
		public function Heavy():void
		{
			super();
			roundType = "Heavy";
			rounds = 1;
			coolDown = 1000;
			jerkVal = -25;
			rangeOfMotion = 20;
			speedOfMotion = 2;
			tip_mc = tipmc;
			chamber_mc = chambermc;
			setup();
		}
		
	}
	
}