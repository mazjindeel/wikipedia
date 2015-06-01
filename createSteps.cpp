
    //take count of from_id's for every to_id (first degree links to page)
    //CREATE TABLE IF NOT EXISTS Counts (id INTEGER PRIMARY KEY, page_id INTEGER, count INTEGER);
    //insert into Counts(count, page_id) select count(link_from_id) as fromCount, link_to_id FROM Links GROUP BY link_to_id ORDER BY fromCount;

    //and put counts in a table
