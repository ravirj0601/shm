/**
 * Server IP address for data exchange.
 * @type {string}
 */
const SERVER_IP = "127.0.0.1";

/**
 * Port number for the server.
 * @type {string}
 */
const SERVER_PORT = "5555";

/**
 * URL for the server using the TCP protocol.
 * @type {string}
 * @example
 * const SERVER_URL = 'tcp://localhost:5555';
 */
const SERVER_URL = `tcp://${SERVER_IP}:${SERVER_PORT}`;

/**
 * Constants related to Data exchange server key name.
 * @namespace
 * @property {number} IMU - Data exchange server key name for IMU.
 * @property {number} PRESSURE_SENSOR - Data exchange server key name for pressure sensors.
 * @property {number} GPS - Data exchange server key name for GPS.
 * @property {number} DVL - Data exchange server key name for DVL.
 */
const SENSORLIST = {
  IMU: "IMU",
  PRESSURE_SENSOR: "PressureSensor",
  GPS: "GPS",
  DVL: "DVL"
};

/**
 * Constants related to index values.
 * @namespace
 * @property {number} IMU_MAX - Maximum value for IMU.
 * @property {number} PRESSURE_SENSOR_MAX - Maximum value for pressure sensors.
 * @property {number} GPS_MAX - Maximum value for GPS.
 * @property {number} DVL_MAX - Maximum value for DVL.
 */
const INDEX = {
  IMU_MAX: 3,
  PRESSURE_SENSOR_MAX: 5,
  GPS_MAX: 4,
  DVL_MAX: 14
};

/**
 * Exported module with frozen properties.
 * @module
 * @property {string} SERVER_URL - URL for the server.
 * @property {Object} INDEX - Constants related to index values.
 */
module.exports = Object.freeze({
  SERVER_URL: SERVER_URL,
  SENSORLIST:SENSORLIST,
  INDEX: INDEX,
});
