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
;
; A function inserting the initial header for a 
; gift file
;
;---------------------------------------------------------------------

(defun gift-file-name-to-define (in-file-name)
  (concat "_"
	  (upcase (replace-in-string in-file-name "\\..*" "" )))
)


(defun  gift-initial-header ()
  "Bracket the current file into #ifndefs to avoid double inclusion"
  (interactive)
  (progn
    (beginning-of-buffer)
    (insert "// -*- mode: c++ -*-\n")
    (insert (concat "#ifndef " (gift-file-name-to-define (buffer-name)) "\n"))
    (insert (concat "#define " (gift-file-name-to-define (buffer-name)) "\n"))
    (end-of-buffer)
    (insert "#endif\n")
    )
  )

(defun gift-prototype-to-comment ()
  "generate an empty comment for this function"
  (interactive)
  (progn
    (re-search-forward "::")
    (kill-word 1)
    (yank)
    (previous-line 1)
    (insert "\n\n/** \n")
    (yank)
    (insert ": \n*/ \n")
    )
)
(defun gift-header-to-c++ ()
  "make a new .cc file to a given header file"
  (interactive)
  (pprint "There is nothing yet"))

