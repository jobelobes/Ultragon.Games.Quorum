package quorum.ship.turret
{
	
	import flash.display.MovieClip;
	
	public class Swivel extends Turret
	{
		
		public function Swivel():void
		{
			super();
			roundType = "Swivel";
			rounds = 100;
			coolDown = 10;
			jerkVal = -0.015;
			rangeOfMotion = 45;
			speedOfMotion = 15;
			tip_mc = tipmc;
			chamber_mc = chambermc;
			setup();
		}
		
	}
	
}