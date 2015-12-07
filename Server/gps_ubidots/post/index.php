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

    $query = "UPDATE `camera`
        SET latitude = '$latitude', longitude = '$longitude', update_time = now()
        WHERE id = '1'";

    $result = mysql_query($query);

    deliver_response(200, "The gps_ubidots commands have been updated", NULL);
    post_data_to_ubidots($latitude, $longitude);

	function post_data_to_ubidots($latitude, $longitude){
        $data['value'] = 1;
        $context['lat'] = $latitude;
        $context['lng'] = $longitude;
        $data['context'] = $context;
        $json_data=json_encode($data);
        $url = "http://things.ubidots.com/api/v1.6/variables/5663284176254275509fc2ad/values";
        $headers = array("Content-Type: application/json", "Accept: application/json; indent=4", "X-Auth-Token: 55QTnmUYv4kLQERtSqD3XAa27n8qnX");
        $ch = curl_init();
        $timeout = 30;
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
        curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $json_data);
        $server_output = curl_exec($ch);
        curl_close($ch);
    }

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
