package quorum.ship.turret
{

	
	public class SwivelBullet extends Bullet
	{
		public function SwivelBullet(_owner:Turret):void {
			super(_owner);
			range = 500;
			damage = 30;
			muzzleVelocity = 10;
			
		}
	}
}