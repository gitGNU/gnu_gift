;----------------------------------------
;
; gift-wn-pos 
;


(global-set-key '(control section) 'gift-wn-pos)

(defun gift-wn-pos ()
  "find a word_v_ or word_n_ combination and run wordnet on it"
  (interactive)
  (progn
    (re-search-backward "_")
    (backward-char)
    (re-search-backward "_")
    (backward-char)
    (re-search-backward "\\b")

    (re-search-backward "\\b");\\( \\|\n\\)
    ;(forward-char)
    (zap-to-char 1 (string-to-char " "))
    (yank)
    (let
	((gift-kind-of-word (cadr (split-string (current-kill 0) "_")))
	 (gift-word (car (split-string (current-kill 0) "_"))))
      (gift-wn-word gift-kind-of-word gift-word)
      )))

(defun gift-wn-word (gift-kind-of-word gift-word)
  (shell-command
   (concat "wn " gift-word (cond ((string-equal gift-kind-of-word
					     "n"
					     ) " -hypen ")
			      ((string-equal gift-kind-of-word
					     "v"
					     ) " -hypev ")
			      ((string-equal gift-kind-of-word
					     "a"
					     ) " -hypea ")
			      )
	   )
   )
  )
