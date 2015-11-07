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

    //$uploadpath = "files/";
    //$filedata = $_FILES['file']['tmp_name'];
    //echo "filedata= ".$filedata;
    //if ($filedata != ''){
        //copy($filedata,$uploadpath."1.c");
        //echo "success";
    //}
?>
