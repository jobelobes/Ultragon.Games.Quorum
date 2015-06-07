package com.as3toolkit.editor {
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.MovieClip;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import flash.geom.Rectangle;
	
	import flash.net.URLRequest;
	
	import flash.text.TextField;
	
	public class PaletteTile extends MovieClip {
		
		private var _index:int = 0;		
		private var _sp:SidePanel;
		
		private var _asset:XML;
		public function get Asset():String { return _asset.@name; }
		public function get Path():String { return _asset.@path; }
		public function get Filename():String { return _asset.@filename; }
		public function get ClassAssociation():String { return _asset.@classAssociation; }
		
		
		private var _icon:Bitmap;
		private var _iconData:BitmapData;
		public function get IconData():BitmapData { return _iconData; }

		
		public function PaletteTile(aXML:XML, index:int):void {
			_asset = aXML;
			
			_sp = SidePanel.getInstance();
			_index = index;
			x = 7 + _index%3*61;
			y = 7 + Math.floor(_index/3)*61;
			
			doubleClickEnabled = true;
			
			addEventListener(MouseEvent.DOUBLE_CLICK, onDC);
			addEventListener(MouseEvent.CLICK, onSel);
			
			var loader:Loader = new Loader();
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onLoaded);
			loader.load(new URLRequest(Palette.rootPath + _asset.@path + _asset.@filename));
			
		}
		
		private function onLoaded(e:Event):void
		{
			_iconData = Bitmap(e.target.content).bitmapData;
			_icon = new Bitmap(_iconData, "auto", true);
			
			if(_icon.height > _icon.width){
				_icon.height = 50;
				_icon.scaleX = _icon.scaleY;
			} else {
				_icon.width = 50;
				_icon.scaleY = _icon.scaleX;
			}
			
			addChild(_icon);
			
			_icon.x = (55.2 - _icon.width)/2;
			_icon.y = (55.2 - _icon.height)/2;
			
			Palette.registerTexture(this);
		}
		
		
		private function onSel(e:MouseEvent):void {
			//trace("CLICK");
			_sp.selectTile(this);
		}
		
		private function onDC(e:MouseEvent):void {
			//trace("DOUBLE-CLICK");
		}
		
	}
	
}