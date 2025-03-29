# Node client for data exchange

## Dependencies

```bash
Node.js v8.17.0 and npm v0.39.5
```

## Installation

Steps to set up the project

1. Clone the Repository on the target machine and navigate to the project root location.

2. Run the following to install dependencies

```bash
  npm install
```

## To Run the sample code

To run the sample code, execute the following command from the root directory in the repository

```bash
  npm test
```

## Usage

1. Move this entire project directory (including ```node_modules```) to /lib of the target project
2. Import the functions as follows,

```
const getSensorDataArray = require("../index").getSensorDataArray;
```
Note: Modify the URL in the require("path/to/index") accordingly.

3. Prefix function call with ```await```

```
await getSensorDataArray(SENSORLIST.<SENSOR_NAME>, INDEX.<SENSOR_MAX_INDEX>);
```

## Documentation

### `getSensorDataArray(sensor_name,index): Promise<number[]>`
This async function retrieves data from a sensor for all the indices.

- **Parameters:**
  - `sensor_name` : The name of sensors to retrieve data from.
  - `index` : The number of indices to retrieve data for a sensor. Note that, the starting index can be ignored as it holds `NaN` padding.

- **Returns:** A Promise that resolves to an array of numbers representing the sensor data. The array includes NaN as the first element.

## Contributors

  1. Ashok Paramasivam B <<ashok.b@planystech.com>>