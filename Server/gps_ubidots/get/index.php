<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);

    $query = "SELECT *
        FROM gps_ubidots
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $face_direction = $result_array[face_direction];
    $latitude = $result_array[latitude];
    $longitude = $result_array[longitude];
    $update_time = $result_array[update_time];

    $response['face_direction'] = $face_direction;
    $response['latitude'] = $latitude;
    $response['longitude'] = $longitude;
    $response['update_time'] = $update_time;

    deliver_response(200, "The gps_ubidots commands have been got", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>