package quorum.ship.turret
{

	
	public class HeavyBullet extends Bullet
	{
		public function HeavyBullet(_owner:Turret):void {
			super(_owner);
			range = 500;
			damage = 60;
			muzzleVelocity = 6;
			
		}
	}
}