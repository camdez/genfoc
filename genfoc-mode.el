(defvar genfoc-mode-hook nil)

(defvar genfoc-mode-map
  (let ((genfoc-mode-map (make-keymap)))
    (define-key genfoc-mode-map "\C-j" 'newline-and-indent)
    genfoc-mode-map)
  "Keymap for genfoc major mode")

(defconst genfoc-font-lock-keywords-1
  (list
   '("^[ \\t]*#.*$" . font-lock-comment-face)
   '("\\[\\w+\\]" . font-lock-constant-face)
   '("^\\w+$" . font-lock-variable-name-face)
   '("\\(^\\|[ \\t]*\\)\\$\\w+" . font-lock-variable-name-face)
   '("^[ \\t]*\\~\\w+" . font-lock-variable-name-face))
  "Minimal highlighting expressions for genfoc mode")

(defvar genfoc-font-lock-keywords genfoc-font-lock-keywords-1
  "Default highlighting expressions for genfoc mode")

(defvar genfoc-mode-syntax-table
  (let ((genfoc-mode-syntax-table (make-syntax-table)))
    (modify-syntax-entry ?_ "w" genfoc-mode-syntax-table)
    genfoc-mode-syntax-table)
  "Syntax table for genfoc-mode")

;;; need to add syntax table setting
(defun genfoc-mode ()
  "Major mode for editing Cam Desautels's Focus Generator data files"
  (interactive)
  (kill-all-local-variables)
  (use-local-map genfoc-mode-map)
  (set (make-local-variable 'font-lock-defaults) '(genfoc-font-lock-keywords))
  (set-syntax-table genfoc-mode-syntax-table)
  (setq major-mode 'genfoc-mode)
  (setq mode-name "GenFoc")
  (run-hooks 'genfoc-mode-hook))


(provide 'genfoc-mode)
