// - - - - - - - - : datas


// (C) Copyright by zHypezinXzL(R)

// @include      : a_xml
// @version      : ( 1 )
// @infoV        : ( 1.4.15.1 ) : discriminator.define(s).function(s).codchange(s) - note: if the information from a new update is repeated in this version, discriminator(version) ++
// @github       : https://github.com/zHypezinXzL
// @date         : 24th jan. 2024

// @author       : zHypezinXzL
// @contributors : -
// @sent by      : zHypezinXzL
// @codelock     : this code is open for modifications, but it is necessary to have my credits!

// @comments     : 

//    This include was not created with the intention of being a save (player accounts), 
//    it was created just for "tests", and one of the reasons for it not being used for "important saves"
//    is that its ping fluctuates a lot in relation to to how many fields it has, 
//    if someone wants to use it for "simple saves" they can use it with ease.


// - - - - - - - - : include


// custom include-guard to ensure we don't duplicate
#if (defined _HYPE_INC_a_xml)
    // for because it is already defined in another environment
    #endinput 
#endif

// define the include (indentifier)
#define _HYPE_INC_a_xml

// Port to open.mp and loads default variables as INVALID_PLAYER_TEXT_DRAW. (If they are using samp-stdlib)
#tryinclude <open.mp>

#if (!defined _INC_open_mp)
    #tryinclude <a_samp>
#endif


// - - - - - - - - : natives


/// functions


/**
native xml_create_document (document [], declaration_name [], declaration []);
native xml_document_exist (document []);
native xml_delete_document (document []);

native xml_rename_document (document [], new_name []);
native xml_copy_document (document [], copy []);

native xml_create_node (document [], path [], node []); 

native xml_node_set_value_string (document [], path [], node [], value []);
native xml_node_set_value_integer (document [], path [], node [], value);
native xml_node_set_value_floating (document [], path [], node [], Float: value);
native xml_node_set_value_boolean (document [], path [], node [], bool: value);

native xml_node_get_value_string (document [], path [], node []);
native xml_node_get_value_integer (document [], path [], node []);
native xml_node_get_value_floating (document [], path [], node []);
native xml_node_get_value_boolean (document [], path [], node []);
 */


// - - - - - - - - : defines


//
//              I recommend not changing the values ​​below
//


// the maximum number of characters the field value can contain - 768 to normal
#if !defined MAX_XML_VALUE_CHARS
	#define MAX_XML_VALUE_CHARS \
        (768)
#endif

// the maximum number of characters the node can contain - 32 to normal
#if !defined MAX_XML_NODE_CHARS
	#define MAX_XML_NODE_CHARS \
        (32)
#endif

// the maximum number of characters the path can contain - 256 to normal
#if !defined MAX_XML_PATH_CHARS
	#define MAX_XML_PATH_CHARS \
        (256)
#endif

// the maximum number of characters that the document can have per line - 1024 to normal [ don't increase ]
#if !defined MAX_XML_LINE_CHARS
	#define MAX_XML_LINE_CHARS \
        (1024)
#endif


// - - - - - - - - : functions


/**
 *
 *
 *
 */
#define xml_document_exist \
    fexist

/**
 *
 *
 *
 */
#define xml_delete_document \
    fremove

/**
 *
 *
 *
 */
stock xml_create_document (
    const document_name [],
    const declaration_name [],
    const declaration []
) {
    if (!fexist (document_name)) {
        new 
            File: xml_document
        ;
        xml_document = fopen (document_name);
        if (xml_document) {
            if (declaration_name [0]) {
                fwrite (xml_document, "<?xml ");
                fwrite (xml_document, declaration_name);
                fwrite (xml_document, "=\"");
                fwrite (xml_document, declaration);
                fwrite (xml_document, "\"?>\r\n");
            }
            if (fclose (xml_document)) {
                return
                    1
                ;
            }
        }
    }
    return 
        0
    ;
}

/**
 *
 *
 *
 */
stock xml_rename_document (
    const document_name [],
    const new_document_name []
) {
    if (fexist (document_name)) {
        if (!fexist (new_document_name)) {
            if (xml_copy_document (document_name, new_document_name)) {
                return fremove (document_name);
            }
        }
    }
    return 
        0
    ;
}

/**
 *
 *
 *
 */
stock xml_copy_document (
    const document_name [], 
    const document_destination [], 
    bool: copy_if_exists = true
) {
    if (fexist (document_name)) {
        if (!copy_if_exists && fexist (document_destination)) {
            // copy_if_exists (false) and already exists
        } else {
            new 
                File: xml_document
            ;
            xml_document = fopen (document_name, io_read);
            if (xml_document) {
                new 
                    File: xml_document_copy
                ;
                xml_document_copy = fopen (document_destination, io_write);
                if (xml_document_copy) {
                    new 
                        xml_result [MAX_XML_LINE_CHARS + 1] = "\0"
                    ;
                    while (fread (xml_document, xml_result)) {
                        fwrite (xml_document_copy, xml_result);       
                    }
                    if (fclose (xml_document)) {
                        if (fclose (xml_document_copy)) {
                            return 
                                true
                            ;
                        }
                    }
                }
            }
        }
    }
    return 
        false
    ;
}


// - - - - - - - - : + + +

/**
 *   example 'node_path'
 *
 *   ------
 *   "
 *   <players>
 *       <accounts>
 *   "
 *   ------
 *
 *   when creating one in the node:
 *
 *   ------
 *   "
 *   <players>
 *       <accounts>
 *           <zHypezinXzL>
 *   "
 *   ------
 *
 *   in the document:
 *
 *   ------
 *   "
 *   <players>
 *       <accounts>
 *           <zHypezinXzL></zHypezinXzL>
 *       </accounts>
 *   </players>
 *   "
 *   ------
 */
stock xml_create_node (
    const document_name [],
    const node_path [] = "",
    const new_node []
) {
    if (fexist (document_name)) {
        if (__xml_verify_ex (document_name, node_path, new_node, false)) {
            if (new_node [0] && (strlen (new_node) + strlen (node_path)) <= MAX_XML_PATH_CHARS) {
                new 
                    File: xml_document
                ;
                xml_document = fopen (document_name, io_read);
                if (xml_document) {
                    new 
                        File: xml_document_copy, 
                        xml_result [MAX_XML_LINE_CHARS + 1] = "\0"
                    ;
                    format (xml_result, sizeof xml_result, "%s.xml_02", document_name);
                    if (fexist (xml_result))
                        fremove (xml_result);
                    xml_document_copy = fopen (xml_result, io_write);
                    if (xml_document_copy) {
                        new 
                            bool: xml_end,
                            xml_tab,
                            xml_node_ex [MAX_XML_NODE_CHARS + 4] = "\0",
                            xml_path_ex [MAX_XML_PATH_CHARS + 1] = "\0"
                        ;
                        xml_end = false;
                        format (xml_path_ex, sizeof xml_path_ex, node_path);
                        while (fread (xml_document, xml_result)) {
                            if (!xml_end && node_path [0]) {
                                if (!xml_node_ex [0]) {
                                    strmid (xml_node_ex, xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                                }
                                if (strfind (xml_result, xml_node_ex, true, 0) == (xml_tab * 4)) {    
                                    strdel (xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                                    xml_tab ++;
                                    if (strfind (xml_path_ex, "<", true, 0) == -1) {
                                        __xml_line_kill (xml_result);
                                        xml_end = true;
                                        strins (xml_node_ex, "/", 1, (MAX_XML_NODE_CHARS + 4));    

                                        if (strfind (xml_result, xml_node_ex, true, 0) != -1) {
                                            new
                                                xml_lines [2]
                                            ;

                                            // ... 1
                                            xml_lines [1] = strfind (xml_result, "<", true, 0);
                                            strmid (xml_node_ex, xml_result, xml_lines [1], strfind (xml_result, ">", true, 0) + 1);
                                            for (xml_lines [0] = 0; xml_lines [0] != xml_lines [1]; xml_lines [0] ++)
                                                fputchar (xml_document_copy, ' ', true);
                                            fwrite (xml_document_copy, xml_node_ex);
                                            fwrite (xml_document_copy, "\r\n");

                                            // ... 2 
                                            while (xml_tab != 0) {
                                                fwrite (xml_document_copy, "    ");
                                                xml_tab --;
                                            }
                                            format (xml_result, sizeof xml_result, "<%s></%s>\r\n", new_node, new_node);
                                            fwrite (xml_document_copy, xml_result); 
                                            
                                            // ... 3
                                            strins (xml_node_ex, "/", 1);
                                            for (xml_lines [0] = 0; xml_lines [0] != xml_lines [1]; xml_lines [0] ++)
                                                fputchar (xml_document_copy, ' ', true);
                                            fwrite (xml_document_copy, xml_node_ex);
                                            fwrite (xml_document_copy, "\r\n");
                                        } else { 
                                            // if there are already other nodes
                                            fwrite (xml_document_copy, xml_result);
                                            fwrite (xml_document_copy, "\r\n");

                                            // ...
                                            while (xml_tab != 0) {
                                                fwrite (xml_document_copy, "    ");
                                                xml_tab --;
                                            }
                                            format (xml_result, sizeof xml_result, "<%s></%s>\r\n", new_node, new_node);
                                            fwrite (xml_document_copy, xml_result); 
                                        }
                                        continue;
                                    } else {
                                        xml_node_ex [0] = EOS;
                                    }
                                }
                            }
                            __xml_line_kill (xml_result);
                            fwrite (xml_document_copy, xml_result); 
                            fwrite (xml_document_copy, "\r\n");
                        }
                        format (xml_result, sizeof xml_result, "<%s>", new_node);
                        if (!xml_end && !xml_node_exist (document_name, "", xml_result)) {
                            format (xml_result, sizeof xml_result, "<%s></%s>", new_node, new_node);
                            fwrite (xml_document_copy, xml_result); 
                            fwrite (xml_document_copy, "\r\n");
                        }
                        if (fclose (xml_document_copy)) {
                            if (fclose (xml_document)) {
                                format (xml_result, sizeof xml_result, "%s.xml_02", document_name);
                                if (xml_copy_document (xml_result, document_name)) {
                                    return fremove (xml_result);
                                }
                            }
                        }
                    }
                } 
            }
        }
    }
    return 
        0
    ;
}


/**
 *   example 'node_path'
 *
 *   ------
 *   "
 *   <players>
 *       <accounts>
 *   "
 *   ------
 */
stock xml_node_exist (
    const document_name [],
    const node_path [] = "",
    const node_name [],
    bool: just_path = false
) {
    if (fexist (document_name)) {
        if ((node_name [0] && (strlen (node_path) + strlen (node_name)) <= MAX_XML_PATH_CHARS) || (just_path && strlen (node_path) <= MAX_XML_PATH_CHARS)) {
            new 
                File: xml_document
            ;
            xml_document = fopen (document_name, io_read);
            if (xml_document) {
                new
                    xml_path_ex [MAX_XML_PATH_CHARS + 1],
                    xml_result [MAX_XML_LINE_CHARS + 1],
                    xml_node_ex [MAX_XML_NODE_CHARS + 4],
                    xml_node_end_ex [MAX_XML_NODE_CHARS + 4],
                    xml_tab
                ;
                if (!just_path)
                    format (xml_path_ex, sizeof xml_path_ex, "%s<%s>", node_path, node_name);
                else
                    format (xml_path_ex, sizeof xml_path_ex, "%s", node_path);
                while (fread (xml_document, xml_result)) {
                    // checking to see if it is a node (not in use)
                    //if (strfind (xml_path_ex, "<", true, 0) != -1) { 
                        if (!xml_node_ex [0]) {
                            strmid (xml_node_ex, xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                        }
                        if (strfind (xml_result, xml_node_ex, true, 0) == (xml_tab * 4)) {                           
                            strdel (xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                            format (xml_node_end_ex, sizeof xml_node_end_ex, xml_node_ex);
                            strins (xml_node_end_ex, "/", 1);
                            xml_node_ex [0] = EOS;
                            xml_tab ++;
                            if (strfind (xml_path_ex, "<", true, 0) == -1) {
                                fclose (xml_document);
                                return
                                    true
                                ;
                            }   
                        } else if (strfind (xml_result, xml_node_end_ex, true, 0) == ((xml_tab - 1) * 4)) {
                            fclose (xml_document);
                            return
                                false
                            ;
                        }
                    //}
                }
                fclose (xml_document);
            }
        }
    }
    return 
        false
    ;
}

/**
 *   example 'node_path'
 *
 *   ------
 *   "
 *   <players>
 *       <accounts>
 *   "
 *   ------
 */
stock xml_node_set_value_string (
    const document_name [],
    const node_path [] = "",
    const node_name [],
    const value []
) {
    if (fexist (document_name)) {
        if (__xml_verify_ex (document_name, node_path, node_name, true)) {
            if (node_name [0] && (strlen (node_name) + strlen (node_path)) <= MAX_XML_PATH_CHARS) {
                new
                    File: xml_document
                ;
                xml_document = fopen (document_name, io_read);
                if (xml_document) {
                    new 
                        File: xml_document_copy, 
                        xml_result [MAX_XML_LINE_CHARS + 1] = "\0"
                    ;
                    format (xml_result, sizeof xml_result, "%s.xml_02", document_name);
                    if (fexist (xml_result))
                        fremove (xml_result);
                    xml_document_copy = fopen (xml_result, io_write);
                    if (xml_document_copy) {
                        new 
                            bool: xml_end,
                            xml_tab,
                            xml_node_ex [MAX_XML_NODE_CHARS + 4] = "\0",
                            xml_path_ex [MAX_XML_PATH_CHARS + 1] = "\0"
                        ;
                        xml_end = false;
                        format (xml_path_ex, sizeof xml_path_ex, "%s<%s>", node_path, node_name);
                        while (fread (xml_document, xml_result)) {
                            if (!xml_end) {
                                if (!xml_node_ex [0]) {
                                    strmid (xml_node_ex, xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                                }
                                if (strfind (xml_result, xml_node_ex, true, 0) == (xml_tab * 4)) {    
                                    strdel (xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                                    xml_tab ++;
                                    if (strfind (xml_path_ex, "<", true, 0) == -1) {
                                        __xml_line_kill (xml_result);
                                        xml_end = true;
                                        strins (xml_node_ex, "/", 1);  

                                        if (strfind (xml_result, xml_node_ex, true, 0) != -1) {
                                            new
                                                xml_lines [2]
                                            ;
                                            xml_lines [1] = strfind (xml_result, "<", true, 0);
                                            strmid (xml_node_ex, xml_result, xml_lines [1], strfind (xml_result, ">", true, 0) + 1);
                                            for (xml_lines [0] = 0; xml_lines [0] != xml_lines [1]; xml_lines [0] ++)
                                                fputchar (xml_document_copy, ' ', true);
                                            fwrite (xml_document_copy, xml_node_ex);
                                            format (xml_result, sizeof xml_result, value);
                                            __xml_line_kill (xml_result);
                                            fwrite (xml_document_copy, xml_result);
                                            strins (xml_node_ex, "/", 1); 
                                            fwrite (xml_document_copy, xml_node_ex);
                                            fwrite (xml_document_copy, "\r\n");
                                        } else {
                                            fwrite (xml_document_copy, xml_result);
                                            fwrite (xml_document_copy, "\r\n");
                                        }
                                        continue;
                                    } else {
                                        xml_node_ex [0] = EOS;
                                    }
                                }
                            }
                            __xml_line_kill (xml_result);
                            fwrite (xml_document_copy, xml_result); 
                            fwrite (xml_document_copy, "\r\n");
                        }
                        if (fclose (xml_document_copy)) {
                            if (fclose (xml_document)) {
                                format (xml_result, sizeof xml_result, "%s.xml_02", document_name);
                                if (xml_copy_document (xml_result, document_name)) {
                                    return fremove (xml_result);
                                }
                            }
                        }
                    }
                } 
            }
        }
    }
    return 
        0
    ;
}

stock xml_node_set_value_integer (
    const document_name [],
    const node_path [] = "",
    const node_name [],
    value
) {
    new
        xml_result [12]
    ;
    valstr (xml_result, value);
    return
        xml_node_set_value_string (document_name, node_path, node_name, xml_result)
    ;
}

stock xml_node_set_value_floating (
    const document_name [],
    const node_path [] = "",
    const node_name [],
    Float: value
) {
    new
        xml_result [25]
    ;
    format (xml_result, sizeof xml_result, "%f", value);
    return
        xml_node_set_value_string (document_name, node_path, node_name, xml_result)
    ;
}

stock xml_node_set_value_boolean (
    const document_name [],
    const node_path [] = "",
    const node_name [],
    bool: value
) {
    return
        xml_node_set_value_string (document_name, node_path, node_name, ((value) ? ("true") : ("false")))
    ;
}

/**
 *   example 'node_path'
 *
 *   ------
 *   "
 *   <players>
 *       <accounts>
 *   "
 *   ------
 */
stock xml_node_get_value_string (
    const document_name [],
    const node_path [] = "",
    const node_name []
) {
    new 
        xml_result [MAX_XML_LINE_CHARS + 1]
    ;
    if (fexist (document_name)) {
        if (__xml_verify_ex (document_name, node_path, node_name, true)) {
            if (node_name [0] && (strlen (node_name) + strlen (node_path)) <= MAX_XML_PATH_CHARS) {
                new
                    File: xml_document
                ;
                xml_document = fopen (document_name, io_read);
                if (xml_document) {
                    new 
                        xml_tab,
                        xml_node_ex [MAX_XML_NODE_CHARS + 4] = "\0",
                        xml_path_ex [MAX_XML_PATH_CHARS + 1] = "\0"
                    ;
                    format (xml_path_ex, sizeof xml_path_ex, "%s<%s>", node_path, node_name);
                    while (fread (xml_document, xml_result)) {
                        if (!xml_node_ex [0]) {
                            strmid (xml_node_ex, xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                        }
                        if (strfind (xml_result, xml_node_ex, true, 0) == (xml_tab * 4)) {    
                            strdel (xml_path_ex, strfind (xml_path_ex, "<", true, 0), strfind (xml_path_ex, ">", true, 0) + 1);
                            xml_tab ++;
                            if (strfind (xml_path_ex, "<", true, 0) == -1) {
                                __xml_line_kill (xml_result);
                                
                                strins (xml_node_ex, "/", 1);
                                if (strfind (xml_result, xml_node_ex, true, 0) != -1) {
                                    strmid (xml_result, xml_result, strfind (xml_result, ">", true, 0) + 1, strfind (xml_result, xml_node_ex, true, 0));
                                } else {
                                    xml_result [0] = EOS;
                                }
                                fclose (xml_document);
                                return
                                    xml_result
                                ;
                            } else {
                                xml_node_ex [0] = EOS;
                            }
                        }
                    }
                    xml_result [0] = EOS;
                    fclose (xml_document);
                }
            }
        }
    }
    return
        xml_result
    ;
}

stock xml_node_get_value_integer (
    const document_name [],
    const node_path [] = "",
    const node_name []
) {
    return
        strval (xml_node_get_value_string (document_name, node_path, node_name))
    ;
}

stock xml_node_get_value_floating (
    const document_name [],
    const node_path [] = "",
    const node_name []
) {
    return
        floatstr (xml_node_get_value_string (document_name, node_path, node_name))
    ;
}

stock xml_node_get_value_boolean (
    const document_name [],
    const node_path [] = "",
    const node_name []
) {
    return
        ((!strcmp (xml_node_get_value_string (document_name, node_path, node_name), "true")) ? (true) : (false))
    ;
}

// - - - - - - - - : oters


// function with purpose only to include
stock __xml_verify_ex (
    const document_name [],
    const node_path [],
    const node_name [],
    bool: node_exist = true
) {
    if ((node_path [0] && xml_node_exist (document_name, node_path, "", true)) || !node_path [0]) {
        if ((node_exist) ? (node_name [0] && xml_node_exist (document_name, node_path, node_name)) : ((node_name [0] && !xml_node_exist (document_name, node_path, node_name)) || !node_name [0])) {
            return
                1
            ;
        }
    }
    return 
        0
    ;
}

stock __xml_line_kill (
    string []
) {
    if (string [0]) {
        new
            _filesize;
        _filesize = strlen (string);
        if ((string [_filesize - 1] == '\n') || (string [_filesize - 1] == '\r')) {
            string [_filesize - 1] = 0;
            if (string [0]) {
                if ((string [_filesize - 2] == '\n') || (string [_filesize - 2] == '\r')) {
                    string [_filesize - 2] = 0;
                }
            }
        }
    }
    return ;
}


// - - - - - - - - : hookbacks


public OnGameModeInit (

) {
    print ("                                         ");
    print (" Loading include: a_xml.inc              ");
    print ("                                         ");
    print ("- - - - - - - - - - :                    ");
    print ("                                         ");
    print ("   (C) Copyright by zHypezinXzL (R)      ");
    print ("                                         ");
    print ("    @include      : a_xml                ");
    print ("    @version      :( 1 )                 ");
    print ("    @infoV        :( 1.4.15.1 )          ");
    print ("    @date         : 24th jan. 2024       ");
    print ("                                         ");
    print ("    @author       : zHypezinXzL          ");
    print ("    @contributors : -                    ");
    print ("    @sent by      : zHypezinXzL          ");
    print ("    @team         : Hyped                ");
    print ("                                         ");
    print ("- - - - - - - - - - :                    ");
    print ("                                         ");
    print (" >> a_xml included v.1 (1.4.15.1)        ");
    print (" Loaded.                                 ");
    print ("                                         ");

    #if (defined __OnGameModeInit_a_xml)
        return 
            __OnGameModeInit_a_xml (

            )
        ;
    #else 
        return 
            1
        ;
    #endif
}


// - - - - - - - - : hooks 


#if (defined _ALS_OnGameModeInit)
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif


// - - - - - - - - : + + +


#define OnGameModeInit __OnGameModeInit_a_xml


// - - - - - - - - : + + +


#if (defined __OnGameModeInit_a_xml)
	forward __OnGameModeInit_a_xml (

    );
#endif


// - - - - - - - - : end