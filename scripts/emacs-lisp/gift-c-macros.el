;; -*- mode: emacs-lisp -*-

;    GIFT, a flexible content based image retrieval system.
;    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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
; MACROS FOR THE USE WITH C++
; Author: Wolfgang Müller (wolfgang.mueller@cui.unige.ch)
;
;---------------------------------------------------------------------

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Make set/get functions for a given c++ variable
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun  sub1-gift-make-accessors (); puts the right stuff in the kill ring
  (copy-region-as-kill (-
			(re-search-forward "[a-zA-Z]")
			1)
		       (- (search-forward " " 
					  nil 
					  nil 
					  1)
			  1))
					; get the type name 
					;(assuming there are no spaces in the type
  (copy-region-as-kill (re-search-forward "[a-zA-Z]")
		       (- (search-forward ";" 
					  nil 
					  nil 
					  1)
			  1))
    
					;get the variable name
)
    

(defun sub2-gift-make-accessors (in-setfun in-getfun);
  (let ((l-name (car kill-ring-yank-pointer))
	(l-type (car (cdr kill-ring-yank-pointer))))
    (beginning-of-line)
    (next-line 1)
    (insert "\n")
    (insert "///\nvoid set") 
    (insert l-name)
    (insert "(const ")
    (insert l-type)
    (insert "& in")
    (insert l-name)
    (insert ")")
    (funcall in-setfun l-type l-name)
    (insert "///\n")
    (insert l-type)
    (insert " get")  
    (insert l-name)
    (insert "()const")  
    (funcall in-getfun l-type l-name)
    )
  )



(defun gift-header-set-accessor (in-type in-name)
  (insert ";\n"))

(defun gift-header-get-accessor (in-type in-name)
  (insert ";\n"))

(defun gift-body-set-accessor (in-type in-name)
  (insert "{\n m")
  (insert in-name)
  (insert "= in")
  (insert in-name)
  (insert ";\n};\n\n"))

(defun gift-body-get-accessor (in-type in-name)
  (insert "{\n return m")
  (insert in-name)
  (insert ";\n};\n\n"))
  
(defun  gift-make-header-accessors ()
  "make accessors for variable"
  (interactive)
  (save-excursion	(sub1-gift-make-accessors)
			(sub2-gift-make-accessors  'gift-header-set-accessor
						'gift-header-get-accessor)))

(defun  gift-make-body-accessors ()
  "make accessors for variable"
  (interactive)
  (save-excursion (sub1-gift-make-accessors)
		  (sub2-gift-make-accessors 'gift-body-set-accessor
					 'gift-body-get-accessor)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  For Diff: keep the version A
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defalias 'gift-keepA (read-kbd-macro
"C-s <<<<<<<<<<<<<< SPC variant SPC A C-a 2*<C-k> C-s > > > > > > > > > > > > > > SPC variant SPC B C-a C-s ======= SPC end SPC of SPC combination C-x C-x C-w"))
