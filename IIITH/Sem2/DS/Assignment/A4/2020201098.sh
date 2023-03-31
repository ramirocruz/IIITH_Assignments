
if [ "$3" = "sort" ]
then
python sort_merge_join.py "$@"
else
python hash_join.py "$@"
fi
