<?php
    header("Content-Type:application/json");
    include("process.php");

    $led_status = $_GET['led_status'];
    $led_on_ms = $_GET['led_on_ms'];
    $led_off_ms = $_GET['led_off_ms'];

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "UPDATE led
        SET led_status = '$led_status', led_on_ms = '$led_on_ms', led_off_ms = '$led_off_ms'
        WHERE id = '1'";

    $result = mysql_query($query);

    $query = "SELECT id, led_status, led_on_ms, led_off_ms
        FROM led
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $led_status = $result_array[led_status];
    $led_on_ms = $result_array[led_on_ms];
    $led_off_ms = $result_array[led_off_ms];

    $response['led_status'] = $led_status;
    $response['led_on_ms'] = $led_on_ms;
    $response['led_off_ms'] = $led_off_ms;

    deliver_response(200, "The LED status has been changed on server", $response);
    //$json_response=json_encode($response);
    //echo $json_response;

    //if(!empty($_GET['led_status'])){
        //$led_status = $_GET['led_status'];
        //$led_status = process($led_status);
        //$led_on_ms = $_GET['led_on_ms'];
        //deliver_response(200,"Thank you for your request","Welcome to the GPS part, the led_status is: $led_status, the led_on_ms is: $led_on_ms");
    //}else{
        //deliver_response(400,"Bad Request",NULL);
    //}

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
