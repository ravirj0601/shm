const zmq = require("zeromq");

/**
 * The URL of the DEX server for communication.
 * @type {string}
 */
const DEX_SERVER = require("../constants/applications").SERVER_URL;

// socket to talk to server
console.log(`Connecting to ${DEX_SERVER}`);
const requester = zmq.socket("req");

// Establishing connection
requester.connect(DEX_SERVER);

// Register a handler for the SIGINT signal (Ctrl+C) to close the requester when the process is terminated.
process.once("SIGINT", function () {
  requester.close();
});

/**
 * A utility function for requesting sensor data from the server.
 *
 * @param {string} sensor_name - The name of the sensor to request data from.
 * @param {number} index - The index of the sensor data to request.
 * @returns {Promise<string>} A promise that resolves with the received sensor data as a string.
 * @rejects {undefined} If no response is received within 200 milliseconds.
 */
function getSensorData(sensor_name, index) {
  return new Promise((resolve, reject) => {
    // Send a request message to the requester with sensor name and index.
    requester.send(`[R,${sensor_name},${index},0.0]`);
    
    // Listen for a single response message from the requester.
    requester.once("message", function (buffer) {
      // Parse the response buffer into a double precision float (float64).
      const dataView = new DataView(
        buffer.buffer,
        buffer.byteOffset,
        buffer.byteLength
      );
      const doubleValue = dataView.getFloat64(0, true); // true for little-endian
      
      // Resolve the promise with the received sensor data as a string.
      resolve(`${doubleValue}`);
    });
    
    // Set a timeout to reject the promise if no response is received within 200 milliseconds.
    setTimeout(() => {
      reject(undefined);
    }, 200);
  });
}

module.exports = {
  getSensorData
};
