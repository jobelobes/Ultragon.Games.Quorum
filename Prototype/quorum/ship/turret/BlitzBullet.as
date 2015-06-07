package quorum.ship.turret
{

	
	public class BlitzBullet extends Bullet
	{
		public function BlitzBullet(_owner:Turret):void {
			super(_owner);
			range = 500;
			damage = 60;
			muzzleVelocity = 10;
			
		}
	}
}