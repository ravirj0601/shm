const getSensorDataArray = require("../index").getSensorDataArray;
const INDEX = require("../../constants/applications").INDEX;
const SENSORLIST = require("../../constants/applications").SENSORLIST;

async function getAllSensorData() {
  while(true) {
    console.log(await getSensorDataArray(SENSORLIST.IMU, INDEX.IMU_MAX));
    console.log(await getSensorDataArray(SENSORLIST.GPS, INDEX.GPS_MAX));
    console.log(await getSensorDataArray(SENSORLIST.DVL, INDEX.DVL_MAX));
    console.log(await getSensorDataArray(SENSORLIST.PRESSURE_SENSOR, INDEX.PRESSURE_SENSOR_MAX));
  }
}

getAllSensorData();