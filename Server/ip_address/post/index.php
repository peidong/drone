<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $ip_address = $_POST['ip_address'];
    $mac_address = $_POST['mac_address'];
    $network_name = $_POST['network_name'];

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);

    for ($i=5; $i>=0; $i--)
    {
        $j = $i + 1;
        $query = "UPDATE ip_address
            SET id = '$j'
            WHERE id = '$i'";

        $result = mysql_query($query);
    }

    $query = "UPDATE ip_address
        SET ip_address = '$ip_address', mac_address = '$mac_address', network_name = '$network_name', update_time = now()
        WHERE id = '1'";

    $result = mysql_query($query);

    $query = "DELETE FROM ip_address
        WHERE id > 5;";

    $result = mysql_query($query);

    deliver_response(200, "The ip_address has been updated", NULL);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
