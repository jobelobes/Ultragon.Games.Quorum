package quorum.ship.turret
{

	
	public class BlazeBullet extends Bullet
	{
		public function BlazeBullet(_owner:Turret):void {
			super(_owner);
			range = 0;
			damage = 60;
			muzzleVelocity = 0;
			
		}
	}
}