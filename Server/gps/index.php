<?php
    header("Content-Type:application/json");
    if(!empty($_GET['latitude'])){
        $latitude = $_GET['latitude'];
        $latitude = process($latitude);
        $longitude = $_GET['longitude'];
        deliver_response(200,"Thank you for your request","Welcome to the GPS part, the latitude is: $latitude, the longitude is: $longitude");
    }else{
        deliver_response(400,"Bad Request",NULL);
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
