#!/bin/bash
# 05-b-5500
# github.com/andy489

cat ~andreystoev/passwd.txt |\
        awk -F : 'BEGIN { printf("<table>\n <tr>\n  <th>%s</th>\n  <th>%s</th>\n  <th>%s</th>\n  <th>%s</th>\n </tr>\n",
        "Username", "Group", "Login Shell", "GECKO") } 
        { printf(" <tr>\n  <td>%s</td>\n  <td>%s</td>\n  <td>%s</td>\n  <td>%s</td>\n </tr>\n",$1, $4, $5, $7) } 
        END { printf("</table>\n") }'
