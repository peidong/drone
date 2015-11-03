<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $image = $_POST['image'];
    $video = $_POST['video'];

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "SELECT *
        FROM camera
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $last_time_image = $result_array[image];
    $last_time_video = $result_array[video];

    if($image == NULL){
        $image = $last_time_image;
    }
    if($video == NULL){
        $video = $last_time_video;
    }

    $query = "UPDATE camera
        SET image = '$image', video = '$video', update_time = now()
        WHERE id = '1'";

    $result = mysql_query($query);

    $query = "SELECT *
        FROM camera
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $image = $result_array[image];
    $video = $result_array[video];
    $update_time = $result_array[update_time];

    $response['image'] = $image;
    $response['video'] = $video;
    $response['update_time'] = $update_time;

    deliver_response(200, "The camera commands have been updated", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>