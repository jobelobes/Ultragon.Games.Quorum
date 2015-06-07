package com.as3toolkit.editor {
	
	import com.as3toolkit.ui.*;
	import com.as3toolkit.xml.XMLLoader;
	
	import flash.desktop.*;
	import flash.display.*;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	
	import flash.filesystem.*;
	
	import flash.geom.Point;
	
	import flash.net.FileFilter;
	import flash.net.FileReference;
	import flash.net.SharedObject;
	
	import flash.text.*;
	
	import flash.ui.Keyboard;
	import flash.ui.Mouse;
	
	import flash.utils.getTimer;
	import flash.utils.Timer;

	public class Editor extends Sprite{
		
		// ---------------Constants---------
		private const VIEWMODE_EDITOR:int = 0;
		private const VIEWMODE_DS:int = 1;
		
		// ---------------For Reference-----
		private static var _instance:Editor;
		
		// ---------------For Library-------
		//private var _assets:Assets;		
		public var _library:MovieClip;
			public function get Library():MovieClip { return _library };
			public function set Library(library:MovieClip):void { _library = library; };
			
		// ---------------For Interface-----
		private var _sp:SidePanel;
		private var _curser:MovieClip;
		private var _cursor:Cursor;
		private var _targ:Point = new Point();
		private var _mess:String = "Click [+] or File > Load XML...";
		private var _prompt:Prompt;
		private var _viewMode:int = VIEWMODE_EDITOR;
		
		// ---------------For Movement------
		private var _keyboarder:Keyboarder;
		private var _moving:Boolean = false;
		public var mover:Mover = new Mover();
		private var _movers:Array = new Array();
		public function get Movers():Array { return _movers; }
		private var _increment:Number = 0;
		private var _translate:Point = new Point();
		private var oldMouse:Point = new Point();
		
		// --------------For NativeWindow------
		private var _window:NativeWindow;
		private var _promptQuitting:Boolean = true;
		
		// ---------------For Menu-----
		private var 	_file:NativeMenu = new NativeMenu();
			//private var _loadLib:NativeMenuItem = new NativeMenuItem("Load Library SWF...");
//			private var _fs0:NativeMenuItem = new NativeMenuItem("",true);
			private var _load:NativeMenuItem = new NativeMenuItem("Load XML...");
			// private var _unload:NativeMenuItem = new NativeMenuItem("Unload XML"); // WAY easier to just restart the program.
			private var _fs1:NativeMenuItem = new NativeMenuItem("",true);
			private var _save:NativeMenuItem = new NativeMenuItem("Save XML...");
			private var _fs2:NativeMenuItem = new NativeMenuItem("",true);
			private var _quit:NativeMenuItem = new NativeMenuItem("Quit");
			
		private var 	_edit:NativeMenu = new NativeMenu();
			private var _robject:NativeMenuItem = new NativeMenuItem("Remove Object");
			private var _rlayer:NativeMenuItem = new NativeMenuItem("Remove Layer");
			private var _es1:NativeMenuItem = new NativeMenuItem("",true);
			private var _deselect:NativeMenuItem = new NativeMenuItem("Clear Active Object");
			
		private var 	_view:NativeMenu = new NativeMenu();
			private var _vlayout:NativeMenuItem = new NativeMenuItem("Editor Mode");
			private var _vds:NativeMenuItem = new NativeMenuItem("DS Mode");
		
		// --------------For File Operations---
		private var _stream:FileStream;
		private var _levelXML:XML;
		
		
		// --------------For Viewer
		private var _canvas:Canvas;
		
		// --------------For Persistence
		private var assetPath:String;
		private var lso:SharedObject = SharedObject.getLocal("quorumEnvironment");
		
		public function Editor():void {
			stage.scaleMode = StageScaleMode.NO_SCALE;
			_instance = this;
			
			_canvas = new Canvas();
			
			_window = this.stage.nativeWindow;
			_window.addEventListener(Event.CLOSING, wait);
			
			_prompt = new Prompt(this);
			_prompt.x = stage.stageWidth/2;
			_prompt.y = stage.stageHeight/2;
			
			_curser = new MovieClip();
			addChild(_curser);
			
			oldMouse.x = stage.mouseX;
			oldMouse.y = stage.mouseY;
			
			_library = new MovieClip();
			addChild(_library);
			
			_keyboarder = new Keyboarder(this);
			
			_sp = new SidePanel();
			addChild(_sp);
			
			_cursor = new Cursor(_curser);
			addChild(_cursor);
			
			buildMenu();
			
			loadAssets();
			
			addEventListener(Event.ENTER_FRAME, enterFrameHandler);
			stage.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMoved);
			
			_canvas.addEventListener(MouseEvent.CLICK, addSymbolToLayer);
			
			var mouseTimer:Timer = new Timer(500,1);
			mouseTimer.addEventListener(TimerEvent.TIMER, hideMouse);
			mouseTimer.start();
		}
		
		public function resize():void {
			_prompt.promptMask_mc.width = stage.stageWidth;
			_prompt.promptMask_mc.height = stage.stageHeight;
			_sp.x = 950 + (stage.stageWidth - 950)/2;
		}
		
		private function loadAssets():void {
			
			/*if(lso.data.assetList != null){
				// check the LSO to see if there were assets elsewhere!
				trace("Loading assetList from "+lso.data.assetList);
				assetPath = lso.data.assetList;
			}
			else // otherwise try to load locally*/
			{
				trace("Attempting to load assetList locally.");
				assetPath = "flashetList.xml";
			}
			var assetLoader:XMLLoader = new XMLLoader(assetPath, this, onAssetsLoaded);
		}
		
		public function onAssetsLoaded(aXml:XML):void
		{
			if(aXml == null)
			{
				// the load failed, we'll have to start from scratch
				trace("assetList failed to load from: "+assetPath);
			}
			else
			{
				trace("assetList successfully loaded from: "+assetPath);
				// it worked, let's bring in those files!
				_sp.Assets.loadAssetsFromXML(aXml);
				_sp.loadLayersFromXML(aXml);
				// and update our LSO
				lso.data.assetList = assetPath;
			}
		}
		
		private function hideMouse(e:TimerEvent):void {
			Mouse.hide();
		}
		
		private function buildMenu():void {
			var nm:NativeMenu = new NativeMenu();
			nm.addSubmenu(_file, "File");
			nm.addSubmenu(_edit, "Edit");
			nm.addSubmenu(_view, "View");
			
			/*_file.addItem(_loadLib);
				_loadLib.keyEquivalent = "l";
				_loadLib.mnemonicIndex = 1;
				_loadLib.addEventListener(Event.SELECT, loadLibrary);
			_file.addItem(_fs0);*/
			_file.addItem(_load);
				_load.enabled = true;
				_load.keyEquivalent = "o";
				_load.mnemonicIndex = 1;
				_load.addEventListener(Event.SELECT, openXML);
			/*// Removed because it is FAR easier to just restart the program.
			_file.addItem(_unload);
				_unload.keyEquivalent = "w";
				_unload.mnemonicIndex = 0;
				_unload.addEventListener(Event.SELECT, closeXML);*/
			_file.addItem(_fs1);
			_file.addItem(_save);
				_save.enabled = true;
				_save.keyEquivalent = "s";
				_save.mnemonicIndex = 0;
				_save.addEventListener(Event.SELECT, saveXML);
			_file.addItem(_fs2);
			_file.addItem(_quit);
				_quit.keyEquivalent = "q";
				_quit.mnemonicIndex = 0;
				_quit.addEventListener(Event.SELECT, quit);
			
			_edit.addItem(_robject);
				_robject.enabled = false;
				_robject.keyEquivalent = "O";
				_robject.mnemonicIndex = 7;
				_robject.addEventListener(Event.SELECT, removeObject);
			_edit.addItem(_rlayer);
				_rlayer.enabled = true;
				_rlayer.keyEquivalent = "L";
				_rlayer.mnemonicIndex = 7;
				_rlayer.addEventListener(Event.SELECT, removeLayer);
			_edit.addItem(_es1);
			_edit.addItem(_deselect);
				_deselect.keyEquivalent = "a";
				_deselect.mnemonicIndex = 6;
				_deselect.addEventListener(Event.SELECT, clearActiveObject);
				_deselect.addEventListener(Event.SELECT, clearAllMovers);
				
			_view.addItem(_vlayout);
				_vlayout.enabled = false;
				_vlayout.keyEquivalent = "e";
				_vlayout.mnemonicIndex = 0;
				//_vlayout.addEventListener(Event.SELECT, toggleView);
			_view.addItem(_vds);
				_vds.keyEquivalent = "d";
				_vds.mnemonicIndex = 0;
				//_vds.addEventListener(Event.SELECT, toggleView);
			
			// Where should I put this menu, by operating system:
			if (NativeApplication.supportsMenu){
				//OSX
				NativeApplication.nativeApplication.menu = nm;
			} else if(NativeWindow.supportsMenu){
				//WINDOWS
				this.stage.nativeWindow.menu = nm;
			}
		}
		
		// DISPLAY A PROMPT TO NOTIFY THE USER OR REQUEST AN ACTION
		public function displayPrompt(type:String, args:Array):void {
			stage.addChild(_prompt);
			_prompt.display(type, args);
		}
		
		// USER SELECTS LOAD LIBRARY SWF FROM FILE MENU, PRESSES CTRL + L
		private function loadLibrary(e:Event):void{
			var file:File = new File();
			file.browseForOpen("Select Library SWF...", [new FileFilter(".swf", "*.swf")]);
			file.addEventListener(Event.SELECT, loadAssetsFromSWF);
		}
		
		/*private function loadAssetsFromSWF(e:Event):void {
			//appDir is relative to application.xml file!
			var file:File = File.applicationDirectory;
			file.resolvePath("./");
			var lib:File = new File();
			lib.resolvePath((e.target as File).nativePath);
			var relPath:String = file.getRelativePath(lib, true);
			
			_assets.addEventListener("ASSETS_LOADED", assetsLoaded);
			_assets.loadAssetsFromSWF(relPath);//(e.target as File).url);
		}*/
		
		// HARDCODE is BAD
		private function loadAssetsFromSWF(file:String):void {
			//_assets.addEventListener("ASSETS_LOADED", assetsLoaded);
			//_assets.loadAssetsFromSWF(file);//(e.target as File).url);
		}
		
		private function assetsLoaded(e:Event):void {
			//_loadLib.enabled = false;
			_load.enabled = true;
			_mess = "Click [+] or File > Load XML...";
			_targ.x = 921.9;
			_targ.y = 441.9;
		}
		
		public function quickenQuit():void {
			_promptQuitting = false;
		}
		
		// USER SELECTS LOAD XML FROM FILE MENU, PRESSES CTRL + O
		private function openXML(e:Event):void{
			var file:File = new File();
			file.browseForOpen("Select XML File...", [new FileFilter("Text", "*.xml")]);
			file.addEventListener(Event.SELECT, onXMLSelected);
		}
		
		private function onXMLSelected(e:Event):void {
			var xmlLoader = new XMLLoader((e.target as File).url, this, onXMLLoaded);
		}
		
		public function onXMLLoaded(levelXML:XML):void {
			_levelXML = levelXML;
			if(_levelXML.@validInvasionCute == true){
				//setupRenderer();
			} else {
				var title:String = "XML ERROR...";
				var message:String = "Sorry, but the XML you tried to load was invalid in some way. Please select a different file, or complain to Joe.";
				displayPrompt("com.as3toolkit.editor.NotifyPrompt", [title, message]);
			}
		}
		
		// USER SELECTS UNLOAD XML FROM FILE MENU, PRESSES CTRL + W
		/* // Removed because it is FAR easier to just restart the program.
		private function closeXML (e:Event):void {
			writeXML();
		}*/
		
		// USER SELECTS SAVE XML FROM FILE MENU, PRESSES CTRL + S
		public function saveXML (e:Event):void {
			var file:File = new File();
			file.browseForSave("Save Level XML As...");
			file.addEventListener(Event.SELECT, onSaveSelected);
		}
			
		private function onSaveSelected(e:Event):void{
			_levelXML = writeXML();
			if(_levelXML){
				var newXMLStr:String = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + _levelXML.toXMLString();
				var fs:FileStream = new FileStream();
				fs.open((e.target as File), FileMode.WRITE);
				fs.writeUTFBytes(newXMLStr);
				fs.close();
			}
		}
		
		private function writeXML():XML {
			var newXML:XML = new XML('<quorum path="'+Palette.rootPath+'"></quorum>');
			
			var layer:XML;
			var asset:XML;
			var itemTag:XML;
			var position:XML;
			var count:Number = 0;
			
			for each(var ast:PaletteTile in _sp.Assets.Assets)
			{
				asset = <{"asset"} {"name"}={ast.Asset} {"path"}={ast.Path} {"filename"}={ast.Filename} {"classAssociation"}={ast.ClassAssociation}></{"asset"}>
				newXML.appendChild(asset);
			}
			
			for each(var lay:Layer in _sp.Layers) {
				layer = <{"layer"} {"name"}={lay.label_txt.text} {"parallax"}={lay.Parallax}></{"layer"}>
				for each(var item:Item in lay.Items) {
					//ele = lay.AssocLayer.getChildAt(dlo);
					itemTag = <{"item"} {"assetName"}={item.AssetName} {"x"}={item._x} {"y"}={item._y} {"uniqueName"}={item.AssetName+"_"+count}/>
					layer.appendChild(itemTag);
					count++;
				}
				newXML.appendChild(layer);
			}
			
			layer = <{"world"}/>;
			
			var scale:Number = 0;
			
			for each(lay in _sp.Layers){
				for each(item in lay.Items) {
					var tempString:String = item.AssetName;
					
					trace(tempString);
					
					if(item.AssetName == "Planet"){
						scale = 30;
					}else if(item.AssetName == "Asteroid"){
						scale = 5;
					}else if(item.AssetName.indexOf( "Nebula") > -1){
						scale = 50;
					}
					
					if(item.AssetName.indexOf( "Nebula") > -1){
						tempString = "Nebula";
					}
					
					itemTag = <{"environmentObject"} {"type"}={tempString} {"scaleX"}={item._scaleX * scale} {"scaleY"}={item._scaleY * scale} {"positionX"}={item._x / 10} {"positionY"}={item._y / 10} />;
					layer.appendChild(itemTag);
				}
			}
			newXML.appendChild(layer);
			return newXML;
		}
		
		// USER SELECTS QUIT FROM FILE MENU, PRESSES CTRL + Q
		private function quit (e:Event):void {
			_window.dispatchEvent(new Event(Event.CLOSING, true, true));
		}
		
		private function wait (e:Event):void {
			if(_promptQuitting){
				_quit.enabled = false;
				e.preventDefault();
				displayPrompt("com.as3toolkit.editor.RemSavePrompt", null);
				_promptQuitting = false;
			} else {
				_window.close();
			}
		}
		
		public function abort():void {
			_quit.enabled = true;
			_promptQuitting = true;
		}
		
		// USER SELECTS REMOVE OBJECT FROM EDIT MENU, PRESSES CTRL + SHIFT + O
		private function removeObject(e:Event):void {
			if(_movers.length > 0){
				for each(var layer:Layer in _sp.Layers){
					for each(var mover:Item in _movers){
						layer.removeRenderable(mover);
					}
				}
				clearAllMovers(null);
			}
		}
		
		// USER SELECTS REMOVE LAYER FROM EDIT MENU, PRESSES CTRL + SHIFT + L
		private function removeLayer(e:Event):void {
			if(_sp.ActiveLayer != null && _sp.Layers.length > 1){
				_sp.removeLayer();
			}
		}
		
		// USER SELECTS CLEAR ACTIVE OBJECT FROM EDIT MENU, PRESSES CTRL + A
		private function clearActiveObject(e:Event):void {
			_sp.clearActiveTile();
		}
		
		// ALSO:
		private function clearAllMovers(e:Event):void {
			_movers = new Array();
			_robject.enabled = false;
		}
		
		private function onLC(e:*):void {
			//_sp.addXMLLayer(e.xml, e.assoc)
		}
		
		public function addLayer(layer:com.as3toolkit.editor.Layer):void {
			/*if(_sceneManager != null){
				var nl:com.invasioncute.Layer = new com.invasioncute.Layer(new Point(0,0), new Point(1,1));
				_sceneManager.addLayer(nl);
				layer.AssocLayer = nl;
			} else {
				setupRenderer();
			}
			_rlayer.enabled = true;*/
		}
		

		
		private function addSymbolToLayer(e:MouseEvent):void {
			var aX:Number;
			var aY:Number;
			if(_keyboarder.keyIsDown(Keyboard.SPACE) == false && _sp.ActiveTile != null && _sp.ActiveLayer != null){
				
				aX = e.localX + _canvas.position.x * _sp.ActiveLayer.Parallax;
				aY = e.localY - _canvas.position.y * _sp.ActiveLayer.Parallax;
				
				_sp.ActiveLayer.addItem(_sp.ActiveTile.Asset, aX, aY);
				
			} else if (_sp.ActiveLayer != null){
				aX = e.localX + _canvas.position.x * _sp.ActiveLayer.Parallax;
				aY = e.localY - _canvas.position.y * _sp.ActiveLayer.Parallax;
				
				var pt:Point = new Point(aX, aY)
				var child:Item;
				var ul:Point;
				var lr:Point;
				var hw:Number;
				var hh:Number;
				var texture:BitmapData;
				for(var i:int = 0; i<_sp.ActiveLayer.Items.length; i++){
					child = _sp.ActiveLayer.Items[i];
					texture = Palette.getTexture(child.AssetName);
					ul = new Point(child._x, child._y);
					lr = new Point(ul.x + texture.width, ul.y + texture.height);
					if(pt.x > ul.x && 
					   pt.x < lr.x && 
					   pt.y > ul.y && 
					   pt.y < lr.y){
						if(e.altKey){
							removeMover(child);
						} else {
							selectMover(child);
						}
					}
				}
			}
		}
		
		private function selectMover(aItem:Item):void {
			if(_keyboarder.keyIsDown(Keyboard.SHIFT)){
				_movers.push(aItem);
			} else {
				clearAllMovers(null);
				_movers.push(aItem);
			}
			_robject.enabled = true;
		}
		
		private function removeMover(aItem:Item):void {
			var ind:Number = _movers.indexOf(aItem);
			if (ind > -1) {
				_movers.splice(ind,1);
			}
		}
		
		private function onMouseMoved(e:MouseEvent):void {
			var translation:Point = new Point(oldMouse.x - e.stageX, e.stageY - oldMouse.y);
			if(e.buttonDown && _keyboarder.keyIsDown(Keyboard.SPACE)){
				_canvas.position.offset(translation.x, translation.y);
			}
			oldMouse.x = e.stageX;
			oldMouse.y = e.stageY;
		}
		
		private function enterFrameHandler(e:Event):void {
			_curser.x = _cursor.x = stage.mouseX;
			_curser.y = _cursor.y = stage.mouseY;
			
			var layers:Array = _sp.Layers;
			if(layers.length > 0)
			{
				_canvas.erase();
				_canvas.drawBG();
				for (var i:int = layers.length-1; i>-1; i--)
				{
					if(layers[i].Visible)
					{
						_canvas.renderLayer(layers[i]);
					}
				}
			}
			
			if(_sp.ActiveLayer){
				_mess = _sp.ActiveLayer.label_txt.text;
				if(_sp.ActiveTile != null){
					_mess += " :: "+_sp.ActiveTile.Asset;
				} else if(_moving) {
					_mess += " :: Move Mode - Hold Spacebar and Drag";
				}
				_targ.x = (_canvas.mouseX + _canvas.position.x*_sp.ActiveLayer.Parallax);
				_targ.y = (_canvas.mouseY - _canvas.position.y*_sp.ActiveLayer.Parallax);
			}
			
			_cursor.update(_targ, _mess);
			
			if(_movers.length > 0 && _keyboarder != null){
				_increment = 1;
				if(_keyboarder.keyIsDown(Keyboard.SHIFT)){
					_increment = 10;
				}
				
				_translate = new Point();
				
				if(_keyboarder.keyIsDown(Keyboard.RIGHT)){
					_translate.x += _increment;
				}
				
				if(_keyboarder.keyIsDown(Keyboard.LEFT)){
					_translate.x -= _increment;
				}
				
				if(_keyboarder.keyIsDown(Keyboard.UP)){
					_translate.y -= _increment;
				}
				
				if(_keyboarder.keyIsDown(Keyboard.DOWN)){
					_translate.y += _increment;
				}
				
				for each(var movee:Item in _movers){
					movee.trans(_translate);
				}
					
			}
		}
		
		public static function getInstance():Editor {
			return _instance;
		}
		
		public function setCursor(to:BitmapData):void {
			if(to == null){
				_moving = true;
			} else {
				_moving = false;
				clearAllMovers(null);
			}
			_cursor.swapSymbol(to);
		}
		
		public function addTile(c:Object, i:int):void {
			/*_sp.symbols_mc.addChild(_library);
			var temp = new PaletteTile(c, i);
			_library.addChild(temp);*/
		}
		
	}
	
}