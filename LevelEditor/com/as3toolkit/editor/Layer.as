package com.as3toolkit.editor {
	
	import flash.display.MovieClip;
	
	import flash.events.MouseEvent;
	
	import flash.geom.Rectangle;
	
	import flash.text.TextField;
	
	public class Layer extends MovieClip {
		
		private var _index:int = 0;
		
		private var _items:Array = new Array();
		public function get Items():Array { return _items; }
		
		private var _vis:Boolean = true;
		public function get Visible():Boolean { return _vis; }
		private var _dragging:Boolean = false;
		
		private var _sp:SidePanel;
		
		public var parallax:Number = 1.0;
		public function get Parallax():Number { return parallax; }
		
		//private var _assocLayer:com.invasioncute.Layer;
		//public function get AssocLayer():com.as3toolkit.Layer { return _assocLayer; }
		//public function set AssocLayer(val:com.invasioncute.Layer):void { _assocLayer = val; }
		
		public function Layer(index:int):void {
			_sp = SidePanel.getInstance();
			_index = index;
			y = _index * height;
			label_txt.text = "Layer_"+_index;
			label_txt.mouseEnabled = false;
			doubleClickEnabled = true;
			
			visible_btn.gotoAndStop("green");
			visible_btn.addEventListener(MouseEvent.CLICK, onToggleVisible);
			visible_btn.useHandCursor = true;
			
			dragger_mc.addEventListener(MouseEvent.MOUSE_DOWN, onDraggerDown);
			dragger_mc.useHandCursor = true;
			
			addEventListener(MouseEvent.CLICK, onSel);
			addEventListener(MouseEvent.DOUBLE_CLICK, onDC);
		}
		
		public function XMLinit(axml:XML):void {
			label_txt.text = axml.@name;
			parallax = axml.@parallax;
			//_assocLayer = (assoc as com.invasioncute.Layer);
			var tempItem:Item;
			for each (var item:XML in axml.item)
			{
				tempItem = new Item(item);
				_items.push(tempItem);
			}
		}
		
		public function removeRenderable(item:Item):void {
			var index:int = _items.indexOf(item);
			if(index > -1)
			{
				_items.splice(index, 1);
			}
		}
		
		public function addItem(aName:String, aX:Number, aY:Number):void {
			var tempItem:Item = new Item(null, [aName, aX, aY]);
			_items.push(tempItem);
		}
		
		private function onSel(e:MouseEvent):void {
			_sp.select(this);
		}
		
		private function onDC(e:MouseEvent):void {
			_sp.editLayer(this);
		}
		
		private function onToggleVisible(e:MouseEvent):void {
			e.stopImmediatePropagation();
			_vis = !_vis;
			if(_vis){
				visible_btn.gotoAndStop("green");
				//_assocLayer.Visible = true;
			} else {
				visible_btn.gotoAndStop("red");
				//_assocLayer.Visible = false;
			}
		}
		
		public function setVisible(to:Boolean):void {
			visible_btn.gotoAndStop(to?"green":"red");
			_vis = to;
		}
		
		private function onDraggerDown(e:MouseEvent):void {
			this.startDrag(true, new Rectangle(0,-5,0,parent.height));
			stage.addEventListener(MouseEvent.MOUSE_UP, onDraggerUp);
			_dragging = true;
			_sp.setDragging(this);
		}
		
		private function onDraggerUp(e:MouseEvent):void {
			e.stopImmediatePropagation();
			stage.removeEventListener(MouseEvent.MOUSE_UP, onDraggerUp);
			this.stopDrag();
			_dragging = false;
			_sp.stopDragging();
		}
		
	}
	
}