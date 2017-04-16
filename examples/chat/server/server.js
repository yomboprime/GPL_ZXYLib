
/***
 * 
 * Configuration
 * 
 */

// The server port number
var serverPort = 8080;

//  Maximum number of connections
var maxConnections = 300;

// Max number of messages to store
var maxMessages = 500;

/***
 * End of configuration
 */


net = require( 'net' );

var clients = [];

var messageList = [];

var clientId = 0;
var messageId = 0;

var MESSAGE_TYPE_CONNECT = 0x55;
var MESSAGE_TYPE_QUERY_NUM_ROOMS = 0x01;
var MESSAGE_TYPE_QUERY_ROOM_INFO = 0x02;
var MESSAGE_TYPE_JOIN_ROOM = 0x03;
var MESSAGE_TYPE_QUIT = 0x04;
var MESSAGE_TYPE_QUIT_ROOM = 0x05;
var MESSAGE_TYPE_DATA = 0x06;
var MESSAGE_TYPE_QUERY_DATA = 0x07;
var MESSAGE_TYPE_KEEP_ALIVE = 0x08;

var server = net.createServer( function ( socket ) {

    var client = {
        id: clientId++,
        lastMessageRead: messageId,
        socket: socket 
    };
    
    clients.push( client );
    
    console.log( "Client connected with id = " + client.id );

    var count = 0;

    socket.on( 'data', ( data ) => {

        switch ( data[ 0 ] ) {

            case MESSAGE_TYPE_DATA:
                console.log( "Data message from client with id = " + client.id );
                processDataMessageFromClientModeStar( client, data );
                break;
                
            case MESSAGE_TYPE_QUERY_DATA:
                processQueryDataMessageFromClientModeStar( client, data );
                break;
                
            default:
                console.log( "Unknown message from client with id = " + client.id + " byte = " + data[ 0 ] + ", length = " + data.length );
                // TODO socket.end();
                break;

        }

    } );
    
    socket.on( 'end', () => {
        console.log( "Client disconnected with id = " + client.id );
        var index = clients.indexOf( client );
        if ( index >= 0 ) {
            clients.splice( index, 1 );
        }
    } );
    
} );

server.on( 'listening', function() {
    console.log( "Server listening on port " + serverPort );
} );

server.maxConnections = maxConnections;

server.listen( serverPort );


/*
    Functions
*/

function processQueryDataMessageFromClientModeStar( client, data ) {

    if ( messageList.length === 0 ) {
        client.socket.write( Buffer.from( [ MESSAGE_TYPE_QUERY_DATA, 0 ] ) );
        return;
    }

    // Compute first message to be sent
    var last = client.lastMessageRead;
    var numMessages = 0;
    var lastIndex = 0;
    for ( var i = 0, n = messageList.length; i < n; i++ ) {

        var m = messageList[ i ];
        
        if ( m.id >= last ) {
            numMessages++;
            lastIndex = i;
        }
    }

    if ( numMessages === 0 ) {
        client.socket.write( Buffer.from( [ MESSAGE_TYPE_QUERY_DATA, 0 ] ) );
        return;
    }
    
    if ( numMessages > 255 ) {
        numMessages = 255;
    }
    // Send messages, prepending message type and number of data messages
    var theBuffer = Buffer.from( [ MESSAGE_TYPE_QUERY_DATA, numMessages ] );
    for ( var i = lastIndex, n = messageList.length; i < n; i++ ) {
        theBuffer = Buffer.concat( [ theBuffer, messageList[ i ].data ] );
    }
    client.socket.write( theBuffer );

    client.lastMessageRead = messageId;

    // Compute oldest stored message that has been read by all clients
    var lastMessageRead = messageId;
    for ( var i = 0, n = clients.length; i < n; i++ ) {
        var c = clients[ i ];
        if ( c.lastMessageRead < lastMessageRead ) {
            lastMessageRead = c.lastMessageRead;
        }
    }

    // Delete messages older than that message
    while ( messageList.length > 0 && lastMessageRead > messageList[ 0 ].id ) {
        messageList.shift();
    }

}

function processDataMessageFromClientModeStar( client, data ) {

    // If message queue is full, remove the first half
    if ( messageList.length >= maxMessages ) {
        messageList.splice( 0, Math.floor( messageList.length * 0.5 ) );
    }

    // Store message
    messageList.push( {
        id: messageId++,
        timestamp: Date.now(),
        data: data
    } );
}
