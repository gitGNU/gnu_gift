;; -*- mode: emacs-lisp -*-

;    GIFT, a flexible content based image retrieval system.
;    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

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

(defun  gift-general-compile (file-in-gift-tree target)
  "Find a file and compile it"
  (interactive)
  (progn
    (find-file (gift-de-double-slash (concat (getenv "GIFT") 
					  "/"
					  file-in-gift-tree))
	       )
    (insert " ")
    (delete-backward-char 1)
    (compile (concat "make -k " target))
    )
  )

(defun  gift-server-compile ()
  "Find the file containing the GIFT server, then compile"
  (interactive)
  (progn
    (gift-general-compile "GIFTServer/Server.cc" " ")
    )
  )
