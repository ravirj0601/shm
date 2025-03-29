/**
 *
 * Version Control Block
 * Product: node_dex
 * Version: V1.1.0
 *
 * IMPORTANT NOTICE:
 *
 * This code is provided for using it as a library.
 * If you are not authorized, Do not modify the content of this file.
 * Any unauthorized use or reproduction of this code is strictly prohibited.
 *
 * For inquiries or permissions, please contact the original author(s)
 * Developer(s): Ashok paramasivam B (ashokb@planystech.com)
 *
 */

const getSensorData = require("./dex").getSensorData;

async function getSensorDataArray(sensor_name = null, index = 0) {
    // Initialize an array to store sensor data with NaN as the first element.
  let data_array = [NaN];
  let i = 1;

  if (sensor_name === null || index === 0) return data_array;

  // Iterate through the specified number of times (index parameter).
  while (i <= index) {
    // Await the result of the getSensorData function for the current sensor (i).
    await getSensorData(sensor_name, i)
      .then((res) => {
        // Update the array with the received data for the current sensor (i).
        data_array[i] = res;
      })
      .catch((error) => {
        // If there's an error (e.g., a timeout), set the data for the current sensor (i) to 0.
        data_array[i] = 0;
      });
    // Move to the next sensor (i).
    i++;
  }

  // Return the populated data array.
  return data_array;
}

module.exports = {
  getSensorDataArray,
};
