/*
*  Original Author: Simon Joslin - Oct 2008
*  Rewritten to the taste of Joe Pietruch - Mar 2009
*
*  For use with the XBOX 360 Gamepad Server.exe
*  Server Originally from http://projects.psibaspace.net.au/code.php
*  Server Rewritten by programming genius Mike Ey
*/

package quorum.xbox {
	
	import flash.net.Socket;
	import flash.events.*
	import flash.display.Loader;
	import flash.utils.Endian;
	import flash.utils.ByteArray;
	
	public class XBox360Manager {
		
		private static var allowed:Boolean = false;
		private static var INSTANCE:XBox360Manager = null;
		protected static var STREAMLENGTH:int = 160;
		
		protected var _socket:Socket;
		protected var _ipAddress:String;
		protected var _port:int;
		protected var _padStates:Array;
		protected var _connected:Boolean = false;
		
		// create a buffer bytearray to copy incoming inputs
		protected var _coordsBytes:ByteArray;
		protected var _inputs:ByteArray; 
		
		public static function getInstance():XBox360Manager {
			if(INSTANCE == null){
				allowed = true;
				INSTANCE = new XBox360Manager();
				allowed = false;
			}
			return INSTANCE;
		}		
		
		public function XBox360Manager():void {
			if(!allowed){
				trace("HEY! You're supposed to make me through XBox360Manager.GetInstance();");
			}
		}
		
		public function connect(ipAddress:String = "localhost", port:int = 0x4a54):void
		{
			_ipAddress = ipAddress;
			_port = port;
			
			_padStates = new Array();
			_padStates.push(new XBox360Controller(1));
			_padStates.push(new XBox360Controller(2));
			_padStates.push(new XBox360Controller(3));
			_padStates.push(new XBox360Controller(4));
			
			_coordsBytes = new ByteArray();
			_inputs = new ByteArray();
			
			// if already connected, disconnect
			if (_connected)
			{
				_socket.removeEventListener (Event.CONNECT, onSocketConnected);
				_socket.removeEventListener (ProgressEvent.SOCKET_DATA, onSocketData);
				_socket.removeEventListener (IOErrorEvent.IO_ERROR, onSocketError);
				_socket.close();
				_connected = false;
			}
			_socket = new Socket ();
			_socket.addEventListener (Event.CONNECT, onSocketConnected);
			_socket.addEventListener (ProgressEvent.SOCKET_DATA, onSocketData);
			_socket.addEventListener (IOErrorEvent.IO_ERROR, onSocketError);
			_socket.connect (_ipAddress, _port);
		}
		
		function onSocketConnected ( pEvt:Event ):void
		{
			trace("XBox360Manager connected to server at " + _ipAddress + " on port: " + _port);
			_connected = true;
		}
		
		function onSocketError ( pEvt:IOErrorEvent ):void
		{
			trace("#### XBox360Manager::onSocketError #### ERROR!! is the server running?");
			throw new Error(pEvt.toString());
			_connected = false;
		}
		
		function onSocketData ( pEvt:Event ):void
		{
			// when data comes through the connection, wait till the packet
			// is full then send to parseBytes

			// grab the bytes coming in
			while ( pEvt.target.bytesAvailable > 0 ) 
			{
				_coordsBytes.writeByte ( pEvt.target.readByte() );
				
				// when a full packet is available, parse it
				if ( _coordsBytes.position == XBox360Manager.STREAMLENGTH ) 
				{
					_inputs.position = 0;
					_inputs.writeBytes ( _coordsBytes );
					_coordsBytes.position = _inputs.position = 0;
					parseBytes ( _inputs );
				}
			}
		}

		protected function parseBytes(pInputs:ByteArray ):void 
		{
			// send the relevant portions of the stream to the gamepads.
			//var numPlayers:int = pInputs.readInt();
			var i:int = 0;
			var playerByteArray:ByteArray;
			
			// loop through each player input and send to the gamepad state
			for (i; i<4; i++)
			{
				playerByteArray = new ByteArray();
				pInputs.readBytes(playerByteArray, 0, 38);
				_padStates[i].parseBytes(playerByteArray);
			}
		}
		
		public function getController(playerIndex:int):XBox360Controller
		{
			// return the requested XBox360Controller
			playerIndex;
			if (playerIndex > _padStates.length ||
				playerIndex < 0)
			{
				throw new Error("playerIndex out of range");
			}
			
			return _padStates[playerIndex];
		}

	}//class
}//package