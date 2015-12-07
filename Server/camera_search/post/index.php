<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $location_type = $_GET['location_type'];
    $face_direction = $_POST['face_direction'];
    $latitude = $_POST['latitude'];
    $longitude = $_POST['longitude'];
    $stop_sign = $_POST['stop_sign'];

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "SELECT *
        FROM gps
        WHERE location_type = '$location_type'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $last_time_control_type = $result_array[face_direction];
    $last_time_auto_control_command = $result_array[latitude];
    $last_time_manual_control_command = $result_array[longitude];
    $last_time_stop_sign = $result_array[stop_sign];
    $last_time_update_time = $result_array[update_time];

    if($face_direction == NULL){
        $face_direction = $last_time_control_type;
    }
    if($latitude == NULL){
        $latitude = $last_time_auto_control_command;
    }
    if($longitude == NULL){
        $longitude = $last_time_manual_control_command;
    }
    if($stop_sign == NULL){
        $stop_sign = $last_time_stop_sign;
    }

    $query = "UPDATE gps
        SET face_direction = '$face_direction', latitude = '$latitude', longitude = '$longitude', stop_sign = '$stop_sign', update_time = now()
        WHERE location_type = '$location_type'";

    $result = mysql_query($query);

    $query = "SELECT *
        FROM gps
        WHERE location_type = '$location_type'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $face_direction = $result_array[face_direction];
    $latitude = $result_array[latitude];
    $longitude = $result_array[longitude];
    $stop_sign = $result_array[stop_sign];
    $update_time = $result_array[update_time];

    $response['face_direction'] = $face_direction;
    $response['latitude'] = $latitude;
    $response['longitude'] = $longitude;
    $response['stop_sign'] = $stop_sign;
    $response['update_time'] = $update_time;

    deliver_response(200, "The gps commands have been updated", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
