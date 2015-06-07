package com.as3toolkit.editor {
	
	import flash.display.Sprite;
	
	import flash.geom.Point;
	
	public class Item extends Sprite {
		
		private var _xml:XML;
		
		private var _assetName:String;
		public function get AssetName():String { return _assetName; }
		public var _x:Number;
		public var _y:Number;
		public var _scaleX:Number = 1;
		public var _scaleY:Number = 1;
		private var _uniqueName:String;
		private var _options:Array = new Array();
		
		public function Item(aXML:XML = null, args:Array = null):void {
			//trace("HERE I AM!");
			if(aXML != null) {
				_xml = aXML;
				_assetName = _xml.@assetName;
				_x = _xml.@x;
				_y = _xml.@y;
				_uniqueName = _xml.@uniqueName;
				
				var key:String;
				var value:String;
				for each (var child:XML in _xml.children())
				{
					key = child.name().localName;
					value = child.child(0);
					_options[key] = value;
				}
			} else {
				var xmlString:String = '<item assetName="'+args[0]+'" x="'+args[1]+'" y="'+args[2]+'"></item>'
				_xml = new XML(xmlString);
			}
			
			_assetName = _xml.@assetName;
			_x = _xml.@x;
			_y = _xml.@y;
		}
		
		public function trans(pt:Point):void {
			_xml.@x = _x +=  pt.x;
			_xml.@y = _y +=  pt.y;
		}
		
	}
	
}