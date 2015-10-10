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

    for ($i=4; $i>=1; $i--)
    {
        $j = $i + 1;
        $query = "SELECT id, ip_address, mac_address, network_name, update_time
            FROM ip_address
            WHERE id='$i'";
        $result = mysql_query($query);
        $result_array = mysql_fetch_array($result);
        $query = "UPDATE ip_address
            SET ip_address='$result_array[ip_address]', mac_address='$result_array[mac_address]', network_name='$result_array[network_name]', update_time='$result_array[update_time]'
            WHERE id = '$j'";

        $result = mysql_query($query);
    }

    $query = "UPDATE ip_address
        SET ip_address = '$ip_address', mac_address = '$mac_address', network_name = '$network_name', update_time = now()
        WHERE id = '1'";

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
