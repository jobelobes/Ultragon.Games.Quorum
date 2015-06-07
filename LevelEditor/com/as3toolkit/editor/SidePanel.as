package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class SidePanel extends MovieClip {
		
		private var _layerXML:XML;
		
		private var layers:Array = new Array();
		public function get Layers():Array { return layers; }
		private var _dragger:Layer;
		
		private var _activeLayer:Layer = null;
		public function get ActiveLayer():Layer {return _activeLayer;}
		
		private var _assets:Palette;
		public function get Assets():Palette { return _assets; }
		
		private var _activeTile:PaletteTile = null;
		public function get ActiveTile():PaletteTile {return _activeTile;}
		
		private var _editor:Editor;
		
		private var _up:Boolean = false;
		private var _down:Boolean = false;
		private var _scrollSpeed:Number = 0;
		private static var _instance:SidePanel;
		
		private var assets:XML;
		
		public function SidePanel():void {
			_instance = this;
			_assets = symbols_mc;
			_editor = Editor.getInstance();
			x = 950;
			addLayer_btn.addEventListener(MouseEvent.CLICK, onAddLayer);
			addLayer_btn.buttonMode = true;
			
			up_btn.addEventListener(MouseEvent.MOUSE_OVER, onUpOver);
			up_btn.addEventListener(MouseEvent.MOUSE_OUT, onUpOut);
			
			down_btn.addEventListener(MouseEvent.MOUSE_OVER, onDownOver);
			down_btn.addEventListener(MouseEvent.MOUSE_OUT, onDownOut);
			
			addEventListener(Event.ENTER_FRAME, onEF);
		}
		
		private function onEF(e:Event):void {
			if (_assets.height > 325){
				if(_up){
					_assets.y += _scrollSpeed;
					_scrollSpeed += 0.3;
				} else if(_down) {
					_assets.y += _scrollSpeed;
					_scrollSpeed -= 0.3;
				} else {
					_scrollSpeed = 0;
				}
				if(_assets.y > 38){
					_assets.y = 38;
				} else if (_assets.y < 38 - _assets.height + 325) {
					_assets.y = 38 - _assets.height + 325;
				}
			}
		}
		
		private function onUpOver(e:MouseEvent):void {
			if(_assets.height > 325)
			{
				up_btn.alpha = 1.0;
				_up = true;
			}
		}
		
		private function onUpOut(e:MouseEvent):void {
			if(_assets.height > 325)
			{
				up_btn.alpha = 0.0;
				_up = false;
			}
		}
		
		private function onDownOver(e:MouseEvent):void {
			if(_assets.height > 325)
			{
				down_btn.alpha = 1.0;
				_down = true;
			}
		}
		
		private function onDownOut(e:MouseEvent):void {
			if(_assets.height > 325)
			{
				down_btn.alpha = 0.0;
				_down = false;
			}
		}
		
		public static function getInstance():SidePanel {
			return SidePanel._instance;
		}
		
		public function loadLayersFromXML(aXML:XML):void
		{
			_layerXML = aXML;
			for each (var layer:XML in _layerXML.layer)
			{
				addXMLLayer(layer);
			}
		}
		
		private function onAddLayer(e:MouseEvent)
		{
			if(layers.length > 0){
				var tl:Layer = new Layer(layers.length);
				layers_mc.addChild(tl);
				layers.push(tl);
				_editor.addLayer(tl);
				if(layers.length == 1){
					select(tl);
				}
				editLayer(tl);
			} else {
				_editor.addLayer(null);
			}
		}
		
		public function removeLayer():void {
			layers.splice(layers.indexOf(_activeLayer),1);
			if(layers_mc.contains(_activeLayer)){
				layers_mc.removeChild(_activeLayer);
			}
			stopDragging();
			if(layers.length > 0){
				select(layers[0]);
			}
		}
		
		public function addXMLLayer(axml:XML):void {
			var tl:Layer = new Layer(layers.length);
			layers_mc.addChild(tl);
			layers.push(tl);
			if(layers.length == 1){
				select(tl);
			}
			tl.XMLinit(axml);
		}
		
		public function editLayer(layer:Layer):void {
			_editor.displayPrompt("com.as3toolkit.editor.LayerPrompt", [layer]);
		}
		
		public function select(layer:Layer):void {
			if(_activeLayer != null){
				_activeLayer.gotoAndStop("normal");
			}
			_activeLayer = layer;
			_activeLayer.gotoAndStop("selected");
		}
		
		public function selectTile(tile:PaletteTile):void {
			if (_activeTile != null){
				_activeTile.gotoAndStop("normal");
			}
			_activeTile = tile;
			_activeTile.gotoAndStop("selected");
			_editor.setCursor(_activeTile.IconData);
		}
		
		public function clearActiveTile():void {
			if (_activeTile != null){
				_activeTile.gotoAndStop("normal");
			}
			_activeTile = null;
			_editor.setCursor(null);
		}
		
		public function setDragging(layer:Layer):void {
			addEventListener(Event.ENTER_FRAME, dragFrame);
			_dragger = layer;
			layers_mc.addChild(_dragger);
		}
		
		private function dragFrame(e:Event):void {
			layers = layers.sortOn("y", Array.NUMERIC);
			var tl:Layer;
			var newOrder:Array = new Array();
			for(var i:int = layers.length-1; i>-1; i--){
				tl = layers[i];
				if(tl != _dragger){
					tl.y = i*tl.height;
				}
				//newOrder.push(tl.AssocLayer);
			}
			//_editor.SceneMan.reorderLayers(newOrder);
		}
		
		public function stopDragging():void {
			removeEventListener(Event.ENTER_FRAME, dragFrame);
			_dragger = null;
			var tl:Layer;
			for(var i:int = 0; i<layers.length; i++){
				tl = layers[i];
				tl.y = i*tl.height;
			}
		}
		
		/*public function unloadAll():void {
			while(layers.length > 0)
			{
				for each(var item:Item in layers[0].Items)
				{
					layers[0].removeRenderable(item);
				}
				if(layers_mc.contains(layers[0])){
					layers_mc.removeChild(layers[0]);
				}
				layers.splice(0,1);
			}
			trace("maybe?");
		}*/
		
	}
	
}