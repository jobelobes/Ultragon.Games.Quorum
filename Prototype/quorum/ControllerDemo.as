package quorum {
	
	import quorum.xbox.*;
	
	import flash.display.MovieClip;
	
	public class ControllerDemo extends MovieClip {
		
		private var manager:XBox360Manager;
		private var controllerGraphics:Array = new Array();
		
		public function ControllerDemo():void {
			
			manager = XBox360Manager.getInstance();
			manager.connect();
			
			var tempCG:XBox360ControllerGraphic;
			for(var i:int = 0; i < 4; i++)
			{
				tempCG = new XBox360ControllerGraphic(manager, i);
				addChild(tempCG);
				tempCG.scaleX = tempCG.scaleY = .5;
				tempCG.x = tempCG.width*i;
				controllerGraphics.push(tempCG);
			}
		}
	}
}