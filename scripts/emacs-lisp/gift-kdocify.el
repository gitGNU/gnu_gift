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
; Changing Henning's old comment format to something
; which can be digested by KDOC
;
;---------------------------------------------------------------------

(defun  gift-doc-to-kdoc ()
  "kdocify one-line comments starting at the beginning of the file, unify multiline one-line-comments"
  (beginning-of-buffer)
  (interactive)
  ;turning one-line comments into kdoc comments
  (query-replace-regexp  "//+\\([^-*\n]+\\)" "/** \\1 */")
  ;unify multiline comments into one multiline comment
  (query-replace-regexp  "\\*/[ \t]*\n[ \t]*/\\*\\*" "\n")
)

(defun  gift-kdocify-file ()
  "kdocify starting at the beginning of the file"
  (interactive)
  (beginning-of-buffer)
  (while (re-search-forward  "/\\*\\*\\**" nil t)
    (replace-match "/**" nil nil))
  (beginning-of-buffer)
  (while (re-search-forward  "\\*+/" nil t)
    (replace-match "*/" nil nil))
  (beginning-of-buffer)
  (while (re-search-forward  "^\\*\\*+" nil t)
    (replace-match "*" nil nil))
  (beginning-of-buffer)
  (while (re-search-forward  "///\\(.*\\)" nil t)
    (replace-match "/** \\1 */" nil nil))
  (beginning-of-buffer)
  (gift-doc-to-kdoc)
  )
  
