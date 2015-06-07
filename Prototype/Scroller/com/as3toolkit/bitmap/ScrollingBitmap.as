package com.as3toolkit.bitmap {
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	public class ScrollingBitmap extends Bitmap{
		
		public function ScrollingBitmap(className:Class, aWidth:Number, aHeight:Number):void {
			var bmd:BitmapData = new className(aWidth, aHeight);
			
			super(bmd, "auto", true);
		}
		
		public function scrollByPoint(trans:Point):void {
			var xNeg:Number = trans.x < 0 ? -1:1;
			var yNeg:Number = trans.y < 0 ? -1:1;
			var scrollX:Number = Math.floor(Math.abs(trans.x));
			var scrollY:Number = Math.floor(Math.abs(trans.y));
			
			if(scrollX != 0){
				var scrollBufferX:BitmapData;

				// this works fine for scrollX > 0
				var destPoint:Point = new Point();
				var sourceX:Rectangle = new Rectangle(this.bitmapData.width-scrollX, 0, scrollX, this.bitmapData.height);
				var destX:Rectangle = new Rectangle(0, 0, scrollX, this.bitmapData.height);
				// but swap them if scrollX < 0
				if (xNeg == -1){
					var tempRectX:Rectangle = sourceX;
					sourceX = destX;
					destX = tempRectX;
					
					destPoint.x = this.bitmapData.width-scrollX;
				}
				
				//make a tempX
				scrollBufferX = new BitmapData(scrollX,this.bitmapData.height);
				scrollBufferX.copyPixels(bitmapData, sourceX, new Point());
				//scroll x
				bitmapData.scroll(scrollX*xNeg,0);
				
				//copy the x
				
				bitmapData.copyPixels(scrollBufferX, new Rectangle(0,0,scrollX,this.bitmapData.height), destPoint);
			}
			
			if(scrollY != 0){
				var scrollBufferY:BitmapData;

				// this works fine for scrollX > 0
				var destPointY:Point = new Point();
				var sourceY:Rectangle = new Rectangle(0, this.bitmapData.height-scrollY, this.bitmapData.width, scrollY);
				var destY:Rectangle = new Rectangle(0, 0, this.bitmapData.width, scrollY);
				// but swap them if scrollX < 0
				if (yNeg == -1){
					var tempRectY:Rectangle = sourceY;
					sourceY = destY;
					destY = tempRectY;
					
					destPointY.y = this.bitmapData.height-scrollY;
				}
				
				//make a tempX
				scrollBufferY = new BitmapData(this.bitmapData.width,scrollY);
				scrollBufferY.copyPixels(bitmapData, sourceY, new Point());
				//scroll x
				bitmapData.scroll(0, scrollY*yNeg);
				
				//copy the x
				
				bitmapData.copyPixels(scrollBufferY, new Rectangle(0,0,this.bitmapData.width,scrollY), destPointY);
			}

		}
	}
	
}