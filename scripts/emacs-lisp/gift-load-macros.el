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

;
; Load macros which are useful for editing gift files
;

(defun gift-de-double-slash (in-string) (replace-in-string in-string
							"//" 
							"/"
							))
; for doing things by hand, if necessary:
; (progn (setq gift-script-dir (gift-de-double-slash (concat (getenv "GIFT") "/scripts/emacs-lisp/"))) (load (gift-de-double-slash (concat gift-script-dir "gift-load-macros.el"))))

(load (gift-de-double-slash (concat gift-script-dir "gift-server-compile.el")))
(load (gift-de-double-slash (concat gift-script-dir "gift-compile-current.el")))
(load (gift-de-double-slash (concat gift-script-dir "gift-kdocify.el")))
(load (gift-de-double-slash (concat gift-script-dir "gift-c-macros.el")))
(load (gift-de-double-slash (concat gift-script-dir "gift-initial-header.el")))
(load (gift-de-double-slash (concat gift-script-dir "gift-header-to-body.el")))

(load (concat gift-script-dir "gift-transpose-params.el"))
(load (concat gift-script-dir "gift-add-gpl.el"))