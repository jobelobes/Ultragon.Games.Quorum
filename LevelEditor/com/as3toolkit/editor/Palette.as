package com.as3toolkit.editor {
	
	import flash.display.BitmapData;
	import flash.display.MovieClip;

	import flash.events.MouseEvent;
	
	import flash.geom.Rectangle;
	
	import flash.text.TextField;
	
	public class Palette extends MovieClip {
		
		private static var _textures:Array = new Array();
		
		private var _assetXML:XML;
		private var _assets:Array = new Array();
		public function get Assets():Array { return _assets; }
		private var _loaded:Boolean = false;
		public function get isLoaded():Boolean { return _loaded; };
		
		private static var _errorMap:ErrormapData;
		
		public static var rootPath:String = "";
		
		public function Palette():void {
			_loaded = false;
			_errorMap = new ErrormapData();
		}
		
		public function loadAssetsFromXML(aXML:XML):void 
		{
			_assetXML = aXML;
			rootPath = _assetXML.@path;
			trace(rootPath);
			var pt:PaletteTile;
			for each (var asset:XML in _assetXML.asset)
			{
				pt = new PaletteTile(asset, _assets.length);
				addChild(pt);
				_assets.push(pt);
			}
			_loaded = true;
		}
		
		public static function registerTexture(apt:PaletteTile):void {
			_textures[apt.Asset] = apt.IconData;
		}
		
		public static function getTexture(name:String):BitmapData {
			return _textures[name] ? _textures[name]:_errorMap;
		}
		
	}
	
}