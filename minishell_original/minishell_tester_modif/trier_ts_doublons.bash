for file in *.tests; do
  echo "Traitement du fichier : $file"
  if [ -f "$file" ] && [ -r "$file" ]; then
    sort "$file" | uniq > temp.txt
    if [ $? -eq 0 ]; then
      mv temp.txt "$file"
      if [ $? -eq 0 ]; then
        echo "  --> OK : Les doublons ont été supprimés de '$file'."
      else
        echo "  --> ERREUR : Impossible de renommer le fichier temporaire en '$file'."
      fi
    else
      echo "  --> ERREUR : Le traitement (sort/uniq) a échoué pour '$file'."
      rm -f temp.txt
    fi
  else
    echo "  --> AVERTISSEMENT : '$file' n'existe pas ou n'est pas lisible. Ignoré."
  fi
done
echo "Traitement terminé pour tous les fichiers."