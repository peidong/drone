<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $face_direction = $_POST['face_direction'];
    $latitude = $_POST['latitude'];
    $longitude = $_POST['longitude'];

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);

    $query = "INSERT INTO `gps_ubidots`
        (face_direction, latitude, longitude, update_time)
        VALUES ('$face_direction', '$latitude', '$longitude', now())";

    $result = mysql_query($query);

    deliver_response(200, "The gps_ubidots commands have been updated", NULL);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
