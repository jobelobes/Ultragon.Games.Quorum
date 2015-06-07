package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	
	import flash.geom.Point;
	
	import flash.text.TextField;
	
	public class Cursor extends MovieClip {
		
		private var _symbol:DisplayObject;
		private var _pos:Point = new Point();
		private var _curser:MovieClip;
		
		private const DEGRAD:Number = Math.PI/180;
		private const RADDEG:Number = 180/Math.PI;
		
		public function Cursor(curser:MovieClip):void {
			_curser = curser;
			_curser.mouseEnabled = false;
			_curser.mouseChildren = false;
			mouseEnabled = false;
			mouseChildren = false;
		}
		
		public function update(target:Point, mess:String):void {
			pointer_mc.rotation = Math.atan2(-target.y,-target.x)*RADDEG;
			layer_txt.text = mess;
			
			_pos.x = x;
			_pos.y = y;
			
			var dist:Number = target.length;
			if(dist < 35){
				pointer_mc.scaleX = dist/35;
			} else {
				pointer_mc.scaleX = 1;
			}
			
			//target = _pos.subtract(target);
			x_txt.text = "x: "+Math.round(target.x*1000)/10000;
			y_txt.text = "y: "+Math.round(target.y*1000)/10000;
		}
		
		public function swapSymbol(to:BitmapData):void {
			// remove whatever's there
			if(_symbol != null && _curser.contains(_symbol)){
				_curser.removeChild(_symbol);
				_symbol = null;
			}
			
			// add the new stuff
			if(to != null){
				// either a symbol
				_symbol = new Bitmap(to);
				_curser.addChild(_symbol);
				_symbol.alpha = 0.5;
			} else {
				// or the crosshairs
				_symbol = new Crosshairs();
				_curser.addChild(_symbol);
				_symbol.alpha = 0.5;
			}
		}
		
	}
	
}