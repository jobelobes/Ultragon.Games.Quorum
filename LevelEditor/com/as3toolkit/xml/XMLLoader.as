// AS 3.0 Code for Loading XML from a URL, and calling a function when complete:
// Written by Joe Pietruch - www.jp-design.net
// Part of the (future) AS3 Developer Blog at www.as3toolkit.com

package com.as3toolkit.xml {
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IOErrorEvent;
	
	import flash.net.URLRequest;
	import flash.net.URLLoader;
	
	public class XMLLoader extends EventDispatcher{
		
		private var xml:XML; //the XML object data will be loaded into
		private var loader:URLLoader;
		
		private var owner:*; //the object that creates this XMLLoader, or will run the onload function
		private var onload:Function; //the function called when the xml is loaded
		
		//XMLLoader requires three parameters:
		//  a_path:String is the URL of the external XML document
		//  a_obj:* the object that 'owns' the function to be called when the XML is loaded
		//  a_func:Function the function that will be called when the XML is loaded
		//  EXAMPLE: new XMLLoader("files/text.xml", this, onXMLLoad);
		public function XMLLoader(a_path:String, a_obj:*, a_func:Function){
			xml = new XML();
			owner = a_obj;
			onload = a_func;
			loader = new URLLoader(new URLRequest(a_path));
			trace("Now Loading XML: "+a_path);
			loader.addEventListener(Event.COMPLETE, xmlLoaded);
			loader.addEventListener(IOErrorEvent.IO_ERROR, onError);
		}

		function xmlLoaded(event:Event):void
		{
			xml = XML(loader.data);
			xml.ignoreWhitespace = true;
			trace("- Data loaded for '"+xml.name()+"'");
			onload.call(owner, xml);
			// ^-- runs the function specified by onload on the owner object, passing it the xml loaded
		}
		
		function onError(e:IOErrorEvent):void {
			xml = null;
			trace("- XML IOErrorEvent.Error: " + e);
			onload.call(owner, null);
		}
	}
}