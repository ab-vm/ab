(ns ab-clj.core
  "core compilation library."
  (require [clojure.core :as core]
           [clojure.tools.cli :as cli]
           [clojure.string :as string]
           [clojure.pprint :as pprint]
           [clojure.edn :as edn]
           [clojure.core.match :refer [match]]))

(def ^:dynamic *debug* true)

(defmacro define-language [])

(defmacro define-pass [])

;;
;; loading data
;;

(defn read-from-file [name]
  (with-open [r (java.io.PushbackReader. (clojure.java.io/reader name))]
      (read r)))

;;
;; command line support
;;

(defn str-lines
  "join strings with newlines to make a block of text"
  [& strs]
  (string/join \newline strs))

(def cli-options-list
  [["-h" "--help"]
   ["-p" "--print"]
   ["-s" "--syntax"]])

(defn usage-str [opt-summary]
  (str-lines
    "Usage: ab-clj-compile <input> <output>"
    ""
    "Options:"
    opt-summary))


; (defmulti emit
;   )

(defn handle-def [sym rest out]
  (pprint/pprint sym rest))

(defn compile-statement [s out]
  (match s
    ['def sym & rest] (handle-def sym rest out)))

(defn compile [program out]
  (for [statement program]
    (compile-statement statement out)))

(defn print-bytecodes [name]
  (compile (read-from-file name) ()))

(defn print-syntax [name]
  (pprint/pprint (read-from-file name)))

(defn print-usage [opt-summary]
  (println (usage-str opt-summary)))

(defn parse-cli-args [cli-args]
  (cli/parse-opts cli-args cli-options-list))

(defn handle-args [cli-args]
  (let [{:keys [options summary arguments]} (parse-cli-args cli-args)]
    (cond
      (:help options) (print-usage summary)
      (:print options) (print-bytecodes (arguments 0))
      (:syntax options) (print-syntax (arguments 0)))))

(defn -main [& args]
  (let [action (handle-args args)]
    ; nothing?
    ))
