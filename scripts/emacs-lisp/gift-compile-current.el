;; -*- mode: emacs-lisp -*-

;    GIFT, a flexible content based image retrieval system.
;    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

;    This program is free software; you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation; either version 2 of the License, or
;    (at your option) any later version.

;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.

;    You should have received a copy of the GNU General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

;---------------------------------------------------------------------
;
; Macros FOR THE USE WITH C++
; Author: Wolfgang Müller (wolfgang.mueller@cui.unige.ch)
;
; find the server and compile it
;   (useful when testing)
;
;---------------------------------------------------------------------

(defun  gift-switch-to-associated-c++ ()
  "For a given h file usually the c++ file is in ../cc/"
  (interactive)
  (let (
	(cc-file-other-dir 
	   (replace-in-string (buffer-file-name)
			      "\\(lib.+\\)/include/\\(.*\\)\\.h" 
			      "\\1/cc/\\2.cc"
			      ))
	(cc-file-same-dir 
	   (replace-in-string (buffer-file-name)
			      "\\(.*\\)\\.h" 
			      "\\1.cc"
			      ))
	(pre-cc-file (replace-in-string (buffer-file-name)
					"\\(lib.+\\)/include/\\(.*\\)\\.h" 
					"\\1/cc/\\2.pre-cc"
					))
	(bn (buffer-file-name))
	)
    (if (and (or (string< pre-cc-file bn)
		 (string< bn pre-cc-file))
	     (file-readable-p pre-cc-file)
	     )
	(find-file pre-cc-file)
      (if (file-readable-p cc-file-same-dir)
	  (find-file cc-file-same-dir)
	(find-file cc-file-other-dir))
      )
    )
  )

(defun  gift-switch-to-associated-h ()
  "For a given h file usually the c++ file is in ../include/"
  (interactive)
  (let (
	(include-file-other-dir 
	 (replace-in-string (buffer-file-name)
			    "\\(lib.+\\)/cc/\\(.*\\)\\..*cc" 
			    "\\1/include/\\2.h"
			    ))
	(include-file-same-dir 
	 (replace-in-string (buffer-file-name)
			    "\\(.*\\)\\.cc" 
			    "\\1.h"
			    ))
	)
    (if (file-readable-p include-file-same-dir)
	(find-file include-file-same-dir)
      (find-file include-file-other-dir))
    )
  )


(defun  gift-associated ()
  "For a given file switch to the associated c++/h file"
  (interactive)
  (if (string-match ".*cc" (buffer-file-name)
		    )
      (gift-switch-to-associated-h)
    (gift-switch-to-associated-c++)
    )
  
  )


(defun gift-2 ()
  "After this the frame contains two windows, containing header and cc of the current file"
  (interactive)
  (progn
    (gift-switch-to-associated-h)
    (delete-other-windows)
    (split-window)
    (other-window 1)
    (gift-associated)))

(defun gift-utomake ()
  "Load GIFT's configure.in and run automake. strange name for quicker use without keybindings"
  (interactive)
  (progn
    (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					  "/configure.in")))
    (compile (gift-de-double-slash 
	      (concat "rm config.cache;automake;autoconf;./configure --enable-multi-threading --prefix " 
		      (getenv "GIFT")
		      "/../GIFTInstall")))))

(defun gift-Utomake ()
  "Like gift-utomake, but starts make install afterwards"
  (interactive)
  (progn
    (progn
      (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					    "/GIFTServer/Server.cc")))
      (insert " ")
      (delete-backward-char 1)
      (basic-save-buffer)
      )
    (progn
      (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					    "/configure.in")))
      (compile (gift-de-double-slash 
		(concat "automake;autoconf;./configure " 
			;"--enable-multi-threading "
			"--prefix " 
			(getenv "GIFT")
			"/../GIFTInstall; make install"))))
    ))

(defun gift-Itomake ()
  "Like gift-Utomake, but make internal"
  (interactive)
  (progn
    (progn
      (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					    "/GIFTServer/Server.cc")))
      (insert " ")
      (delete-backward-char 1)
      (basic-save-buffer)
      )
    (progn
      (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					    "/configure.in")))
      (compile (gift-de-double-slash 
		(concat "automake;autoconf;./configure --enable-multi-threading --enable-gift --prefix " 
			(getenv "GIFT")
			"/../GIFTInstall; make install"))))
    ))

(defun gift-GIFTServerMake ()
  "Makes the server without running automake;autoconfig;configure first."
  (interactive)
  (progn
    (progn; touch the server file
      (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					    "/GIFTServer/Server.cc")))
      (insert " ")
      (delete-backward-char 1)
      (basic-save-buffer)
      )
    (progn
      (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					    "/configure.in")))
      (compile "make;make install"))
    )
  )

(defun  gift-compile-current ()
  "Compile the current cc file. switch to it if necessary"
  (interactive)
  (progn
    (if (string-match "\\.java" (buffer-name))
	(compile (concat "javac  " 
			 (buffer-name)))
      (progn 
	(gift-switch-to-associated-c++)      ; this does plain nothing if 
					; the current buffer is not a header
					; in a lib.*/include/ directory
	(compile (concat "make -k  " 
			 (replace-in-string (buffer-name)
					    "\\.\\(pre-\\)?cc" 
					    "\\.o"
					    )))
					; so the last expression will almost
					; always do something useful
	)
      )
    )
  )
(defun  gift-directory-compile-current ()
  "Compile the current cc/ directory. switch to it if necessary"
  (interactive)
  (progn
    (progn 
      (gift-switch-to-associated-c++)      ; this does plain nothing if 
					; the current buffer is not a header
					; in a lib.*/include/ directory
      (compile "make -k all install" 
	       )
					; so the last expression will almost
					; always do something useful
      )
    )
  )
