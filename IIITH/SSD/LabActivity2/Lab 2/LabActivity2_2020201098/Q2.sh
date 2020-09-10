cat $1 | grep .* | awk ' $3 =="work" {$3=good}{print}'

