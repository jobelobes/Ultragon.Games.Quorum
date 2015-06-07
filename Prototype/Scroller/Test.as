package {
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	
	import flash.events.Event;
	
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import com.as3toolkit.bitmap.ScrollingBitmap;
	
	public class Test extends MovieClip {
		var bg:ScrollingBitmap;
		var neb:ScrollingBitmap;
		var fg:ScrollingBitmap;
		
		public function Test():void {
			bg = new ScrollingBitmap(BGStars, 1920, 1080);
			neb = new ScrollingBitmap(Nebula, 1920, 1080);
			neb.alpha = 0.6;
			bg.alpha = 0.6;
			fg = new ScrollingBitmap(FGStars, 1920, 1080);
			addChild(bg);
			//addChild(fg);
			addChild(neb);
			
			addEventListener(Event.ENTER_FRAME, onEF);
		}
		
		
		function onEF(e:Event):void {
			var dir:Point = new Point(stage.mouseX-(stage.stageWidth/2), stage.mouseY-(stage.stageHeight/2));
			dir.normalize(dir.length/500 * 8);
			dir.x *= -1;
			dir.y *= -1;
			bg.scrollByPoint(new Point(dir.x*0.5,dir.y*0.5));
			neb.scrollByPoint(new Point(dir.x*4,dir.y*4));
			fg.scrollByPoint(new Point(dir.x*0.1,dir.y*0.1));
		}
	}
	
}