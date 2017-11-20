(ns ab-clj.compile)

(defn serialize-module [out module]
  (foreach [fn (:functions module)]
     (serialize-fn fn))
   t)

(defn serialize-fn [out module]
  t)

(defn emit
  ([ast])
  ([state ast]))

(defn mangle-symbol
  "mangle a symbol to a utf8-string"
  [symbol]
  (return (str symbol)))

(defn -main [& args]
  (println "test"))
