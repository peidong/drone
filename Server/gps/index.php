<?php
    header("Content-Type:application/json");
    include("process.php");
    if(!empty($_GET['name'])){
        $name = $_GET['name'];
        $name = process($name);
        deliver_response(200,"Thank you for your request","Welcome to the GPS part, $name");
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
