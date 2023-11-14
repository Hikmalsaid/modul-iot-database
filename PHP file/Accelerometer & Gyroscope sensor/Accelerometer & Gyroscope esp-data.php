<!DOCTYPE html>
<html><body>
    <meta http-equiv="refresh" content="5">
<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "id21489495_practice_1";
// REPLACE with Database user
$username = "id21489495_user_1";
// REPLACE with Database user password
$password = "Hijikata@678";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, sensor, location, value1, value2, value3, value4, value5, value6, value7, reading_time FROM SensorData ORDER BY id DESC";

echo '<table cellspacing="5" cellpadding="5">
      <tr> 
        <td>ID</td> 
        <td>Sensor</td> 
        <td>Location</td> 
        <td>Temperatur</td> 
        <td>Akselerasi X</td>
        <td>Akselerasi Y</td>
        <td>Akselerasi Z</td>
        <td>Gyroscope X</td>
        <td>Gyroscope Y</td>
        <td>Gyroscope Z</td>
        <td>Timestamp</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = $row["id"];
        $row_sensor = $row["sensor"];
        $row_location = $row["location"];
        $row_value1 = $row["value1"];
        $row_value2 = $row["value2"]; 
        $row_value3 = $row["value3"];
        $row_value4 = $row["value4"];
        $row_value5 = $row["value5"];
        $row_value6 = $row["value6"];
        $row_value7 = $row["value7"];
        $row_reading_time = $row["reading_time"];
        // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
        //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
      
        // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
        //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time + 4 hours"));
      
        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_sensor . '</td> 
                <td>' . $row_location . '</td> 
                <td>' . $row_value1 . '</td> 
                <td>' . $row_value2 . '</td>
                <td>' . $row_value3 . '</td>
                <td>' . $row_value4 . '</td> 
                <td>' . $row_value5 . '</td> 
                <td>' . $row_value6 . '</td> 
                <td>' . $row_value7 . '</td> 
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
</table>
<button onClick="window.location.reload();">Refresh Page</button>
</body>
</html>