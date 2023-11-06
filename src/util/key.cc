﻿#include "util/key.hpp"
#include <map>
#include <vector>
namespace duskland::util {
const util::key_encoding keymap = {
    {{0x9, {'\t'}},
     {0xa, {'\n'}},
     {0x107, {KEY_BACKSPACE}},
     {0x1b,
      {{
          {0x5b,
           {{
               {0x44, {KEY_LEFT}},
               {0x43, {KEY_RIGHT}},
               {0x42, {KEY_DOWN}},
               {0x41, {KEY_UP}},
               {0x46, {KEY_END}},
               {0x48, {KEY_HOME}},
               {0x5a, util::key{'\t', true}},
               {0x31,
                {{{0x35,
                   {{{0x7e, {KEY_F(5)}},
                     {0x3b,
                      {{
                          {0x32, {{{0x7e, util::key{KEY_F(5), true}}}}},
                          {0x35, {{{0x7e, util::key{KEY_F(5), false, true}}}}},
                          {0x37,
                           {{{0x7e, util::key{KEY_F(5), false, true, true}}}}},
                      }}}}}},
                  {0x37,
                   {{{0x7e, {KEY_F(6)}},
                     {0x3b,
                      {{
                          {0x32, {{{0x7e, util::key{KEY_F(6), true}}}}},
                          {0x35, {{{0x7e, util::key{KEY_F(6), false, true}}}}},
                          {0x33,
                           {{{0x7e, util::key{KEY_F(6), false, false, true}}}}},
                          {0x37,
                           {{{0x7e, util::key{KEY_F(6), false, true, true}}}}},

                      }}}}}},
                  {0x38,
                   {{{0x7e, {KEY_F(7)}},
                     {0x3b,
                      {{
                          {0x32, {{{0x7e, util::key{KEY_F(7), true}}}}},
                          {0x33,
                           {{{0x7e, util::key{KEY_F(7), false, false, true}}}}},

                          {0x35, {{{0x7e, util::key{KEY_F(7), false, true}}}}},
                          {0x37,
                           {{{0x7e, util::key{KEY_F(7), false, true, true}}}}},
                      }}}}}},
                  {0x39,
                   {{{0x7e, {KEY_F(8)}},
                     {0x3b,
                      {{
                          {0x32, {{{0x7e, util::key{KEY_F(8), true}}}}},
                          {0x33,
                           {{{0x7e, util::key{KEY_F(8), false, false, true}}}}},

                          {0x35, {{{0x7e, util::key{KEY_F(8), false, true}}}}},
                          {0x37,
                           {{{0x7e, util::key{KEY_F(8), false, true, true}}}}},
                      }}}}}},
                  {0x3b,
                   {{{0x32,
                      {{
                          {0x44, util::key{KEY_LEFT, true}},
                          {0x43, util::key{KEY_RIGHT, true}},
                          {0x42, util::key{KEY_DOWN, true}},
                          {0x41, util::key{KEY_UP, true}},
                          {0x46, util::key{KEY_END, true}},
                          {0x48, util::key{KEY_HOME, true}},
                          {0x50, util::key{KEY_F(1), true}},
                          {0x51, util::key{KEY_F(2), true}},
                          {0x52, util::key{KEY_F(3), true}},
                          {0x53, util::key{KEY_F(4), true}},
                      }}},
                     {0x33,
                      {{
                          {0x44, util::key{KEY_LEFT, false, false, true}},
                          {0x43, util::key{KEY_RIGHT, false, false, true}},
                          {0x42, util::key{KEY_DOWN, false, false, true}},
                          {0x41, util::key{KEY_UP, false, false, true}},
                          {0x46, util::key{KEY_END, false, false, true}},
                          {0x48, util::key{KEY_HOME, false, false, true}},
                          {0x50, util::key{KEY_F(1), false, false, true}},
                          {0x51, util::key{KEY_F(2), false, false, true}},
                          {0x52, util::key{KEY_F(3), false, false, true}},
                          {0x53, util::key{KEY_F(4), false, false, true}},
                      }}},
                     {0x35,
                      {{
                          {0x44, util::key{KEY_LEFT, false, true}},
                          {0x43, util::key{KEY_RIGHT, false, true}},
                          {0x42, util::key{KEY_DOWN, false, true}},
                          {0x41, util::key{KEY_UP, false, true}},
                          {0x46, util::key{KEY_END, false, true}},
                          {0x48, util::key{KEY_HOME, false, true}},
                          {0x50, util::key{KEY_F(1), false, true}},
                          {0x51, util::key{KEY_F(2), false, true}},
                          {0x52, util::key{KEY_F(3), false, true}},
                          {0x53, util::key{KEY_F(4), false, true}},
                      }}},
                     {0x37,
                      {{
                          {0x44, util::key{KEY_LEFT, false, true, true}},
                          {0x43, util::key{KEY_RIGHT, false, true, true}},
                          {0x42, util::key{KEY_DOWN, false, true, true}},
                          {0x41, util::key{KEY_UP, false, true, true}},
                          {0x48, util::key{KEY_HOME, false, true, true}},
                          {0x46, util::key{KEY_END, false, true, true}},
                          {0x50, util::key{KEY_F(1), false, true, true}},
                          {0x51, util::key{KEY_F(2), false, true, true}},
                          {0x52, util::key{KEY_F(3), false, true, true}},
                          {0x53, util::key{KEY_F(4), false, true, true}},
                      }}}}}}}}},
               {0x32,
                {{
                    {0x30,
                     {{{0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(9), true}}}}},
                            {0x33,
                             {{{0x7e,
                                util::key{KEY_F(9), false, false, true}}}}},

                            {0x35,
                             {{{0x7e, util::key{KEY_F(9), false, true}}}}},
                            {0x37,
                             {{{0x7e,
                                util::key{KEY_F(9), false, true, true}}}}},
                        }}}}}},
                    {0x7e, util::key{KEY_INSERT}},
                    {0x3b,
                     {{
                         {0x32, {{{0x7e, util::key{KEY_INSERT, true}}}}},
                         {0x33,
                          {{{0x7e,
                             util::key{KEY_INSERT, false, false, true}}}}},
                         {0x35, {{{0x7e, util::key{KEY_INSERT, false, true}}}}},
                     }}},
                    {0x30, {{{0x7e, {KEY_F(9)}}}}},
                    {0x31,
                     {{{0x7e, {KEY_F(10)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(10), true}}}}},
                            {0x33,
                             {{{0x7e,
                                util::key{KEY_F(10), false, false, true}}}}},

                            {0x35,
                             {{{0x7e, util::key{KEY_F(10), false, true}}}}},
                            {0x37,
                             {{{0x7e,
                                util::key{KEY_F(10), false, true, true}}}}},
                        }}}}}},
                    {0x33,
                     {{{0x7e, {KEY_F(11)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(11), true}}}}},
                            {0x33,
                             {{{0x7e,
                                util::key{KEY_F(11), false, false, true}}}}},

                            {0x35,
                             {{{0x7e, util::key{KEY_F(11), false, true}}}}},
                            {0x37,
                             {{{0x7e,
                                util::key{KEY_F(11), false, true, true}}}}},
                        }}}}}},
                    {0x34,
                     {{{0x7e, {KEY_F(12)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(12), true}}}}},
                            {0x33,
                             {{{0x7e,
                                util::key{KEY_F(12), false, false, true}}}}},
                            {0x35,
                             {{{0x7e, util::key{KEY_F(12), false, true}}}}},
                            {0x37,
                             {{{0x7e,
                                util::key{KEY_F(12), false, true, true}}}}},
                        }}}}}},
                }}},
               {0x33,
                {{{0x7e, util::key{KEY_DELETE}},
                  {0x3b,
                   {{
                       {0x32, {{{0x7e, util::key{KEY_DELETE, true}}}}},
                       {0x33,
                        {{{0x7e, util::key{KEY_DELETE, false, false, true}}}}},
                       {0x35, {{{0x7e, util::key{KEY_DELETE, false, true}}}}},
                   }}}}}},
               {0x35,
                {{{0x7e, util::key{KEY_PGUP}},
                  {0x3b,
                   {{
                       {0x32, {{{0x7e, util::key{KEY_PGUP, true}}}}},
                       {0x33,
                        {{{0x7e, util::key{KEY_PGUP, false, false, true}}}}},
                       {0x35, {{{0x7e, util::key{KEY_PGUP, false, true}}}}},
                   }}}}}},
               {0x36,
                {{{0x7e, util::key{KEY_PGDN}},
                  {0x3b,
                   {{
                       {0x32, {{{0x7e, util::key{KEY_PGDN, true}}}}},
                       {0x33,
                        {{{0x7e, util::key{KEY_PGDN, false, false, true}}}}},
                       {0x35, {{{0x7e, util::key{KEY_PGDN, false, true}}}}},
                   }}}}}},
           }}},
          {0x4f,
           {{
               {0x50, {KEY_F(1)}},
               {0x51, {KEY_F(2)}},
               {0x52, {KEY_F(3)}},
               {0x53, {KEY_F(4)}},
           }}},
      }}}}};
}