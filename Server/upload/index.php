<?php
//http://www.tutorialscollection.com/php-file-upload-how-to-upload-file-in-php-using-_files-in-script-of-php/
    // start the session
    session_start();
    //$reference_filename = $_GET['reference_filename'];
    $image_filename = $_GET['image_filename'];

    $targetfolder = "files/";
    $targetfolder = $targetfolder.basename($_FILES['file']['name']);
    if(move_uploaded_file($_FILES['file']['tmp_name'], $targetfolder))
    {
        echo "The file ".basename($_FILES['file']['name'])." is uploaded";
    }
    else{
        echo "Problem uploading file";
    }
    // close the session
    session_write_close();
    echo "<br><br>";
    //$command = 'make -C opencv && ./opencv/symbol_detection';
    $command = './opencv/symbol_detection reference.jpg ';
    $command .= $image_filename;

    exec($command, $out, $status);
    $scene_corners_bottom_right_x = $out[0];
    $scene_corners_bottom_right_y = $out[1];
    $scene_corners_top_right_x = $out[2];
    $scene_corners_top_right_y = $out[3];
    $scene_corners_top_left_x = $out[4];
    $scene_corners_top_left_y = $out[5];
    $scene_corners_bottom_left_x = $out[6];
    $scene_corners_bottom_left_y = $out[7];

    print_r($scene_corners_bottom_right_x);    
    echo "<br><br>";
    print_r($scene_corners_bottom_right_y);    
    echo "<br><br>";
    print_r($scene_corners_top_right_x);
    echo "<br><br>";
    print_r($scene_corners_top_right_y);
    echo "<br><br>";
    print_r($scene_corners_top_left_x);
    echo "<br><br>";
    print_r($scene_corners_top_left_y);
    echo "<br><br>";
    print_r($scene_corners_bottom_left_x);
    echo "<br><br>";
    print_r($scene_corners_bottom_left_y);
?>
