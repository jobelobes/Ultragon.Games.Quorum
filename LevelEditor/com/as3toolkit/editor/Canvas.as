package com.as3toolkit.editor {
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;
	
	import flash.events.Event;
	
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	public class Canvas extends Sprite {
		
		private var _editor:Editor;
		private var _width:Number;
		private var _height:Number;
		
		public var position:Point = new Point();
		private var oldStage:Point = new Point(950, 700);
		
		private var bitmap:Bitmap;
		
		private var stars:Stars = new Stars(512, 512);
		
		public function Canvas():void {
			_editor = Editor.getInstance();
			
			
			_width = _editor.stage.stageWidth;
			_height = _editor.stage.stageHeight;

			bitmap = new Bitmap(new BitmapData(_width, _height, true, 0xFFFFFFFF), "auto", true);
			addChild(bitmap);
			_editor.addChild(this);
			
			_editor.stage.addEventListener(Event.RESIZE, onResized);
			
			
		}
		
		public function onResized(e:Event):void {
			var newStage:Point = new Point(stage.stageWidth, stage.stageHeight);
			bitmap.bitmapData = new BitmapData(newStage.x, newStage.y, true, 0xFF000000);
			position.offset(oldStage.x - newStage.x, oldStage.y - newStage.y);
			x = (950 - newStage.x)/2;
			y = (700 - newStage.y)/2;
			_editor.resize();
			oldStage.x = newStage.x;
			oldStage.y = newStage.y;
		}
		
		public function erase():void {
			bitmap.bitmapData.fillRect(new Rectangle(0,0,bitmap.bitmapData.width, bitmap.bitmapData.height),0xFF000000);
		}
		
		public function drawBG():void {
			trace(position.y);
			var upper:Point = new Point();
			
			if(position.x < 0){
				upper.x = Math.ceil(Math.abs(position.x)/512)*-512;
			} else {
				upper.x = Math.floor(Math.abs(position.x)/512)*512;
			}
			
			if(position.y > 0){
				upper.y = Math.ceil(Math.abs(position.y)/512)*-512;
			} else {
				upper.y = Math.floor(Math.abs(position.y)/512)*512;
			}
			for(var i:int = 0; i<5; i++){
				for(var j:int = 0; j<3; j++){
					bitmap.bitmapData.copyPixels(stars, new Rectangle(0,0,512,512), new Point((upper.x + i*512)-position.x,(upper.y + j*512) + position.y), null, null, true);
				}
			}
		}
		
		public function renderLayer(aLayer:Layer):void {
			var tempBMD:BitmapData;
			var drawPoint:Point;
			for each (var item:Item in aLayer.Items) {
				tempBMD = Palette.getTexture(item.AssetName)
				drawPoint = new Point(item._x - position.x*aLayer.Parallax, item._y + position.y*aLayer.Parallax);
				bitmap.bitmapData.copyPixels(tempBMD, new Rectangle(0,0,tempBMD.width, tempBMD.height), drawPoint, null, null, true);
				//bitmapData.draw(tempBMD, new Matrix(1,0,0,1,drawPoint.x,drawPoint.y),null,null,null,true);
				if(_editor.Movers.indexOf(item) > -1)
				{
					bitmap.bitmapData.copyPixels(_editor.mover, new Rectangle(0,0,_editor.mover.width, _editor.mover.height), drawPoint, null, null, true);
				}
			}
		}
	}
}