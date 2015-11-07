<?php
//http://www.tutorialscollection.com/php-file-upload-how-to-upload-file-in-php-using-_files-in-script-of-php/
    // start the session
    session_start();
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
    $command = './opencv/symbol_detection';
    exec($command, $out, $status);
    $scene_borders_1_x = $out[0];
    print_r($out);

    //$uploadpath = "files/";
    //$filedata = $_FILES['file']['tmp_name'];
    //echo "filedata= ".$filedata;
    //if ($filedata != ''){
        //copy($filedata,$uploadpath."1.c");
        //echo "success";
    //}
?>
